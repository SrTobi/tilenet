#include "includes.hpp"
#include "package_loader.hpp"

#include "client/package/std_tile_pool.hpp"
#include "client/package/components/std_tile.hpp"
#include "virtual_package_source.hpp"

#include "utils/xml_helpers.hpp"

#include <rxml/iterators.hpp>
#include <rxml/value.hpp>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <iterator>
#include <algorithm>

namespace client {
namespace pload {

template<typename T>
T cast_default(const string& source, const T& def)
{
	try {
		return boost::lexical_cast<T>(source);
	} catch(boost::bad_lexical_cast&)
	{
		return def;
	}
}

boost::wregex ColorFormat(L"#([a-f0-9]{2}){3,4}");


sf::Color parseColorString(string colstr)
{
	boost::algorithm::trim(colstr);
	boost::to_lower(colstr);

	if(colstr.size())
	{
		if(boost::regex_match(colstr, ColorFormat))
		{
			std::wistringstream iss(colstr.substr(1));
			iss >> std::hex;

			uint32 col;
			iss >> col;

			tnAssert(!iss.fail() && !iss.bad());

			if(colstr.size() == 7)
				col |= TNMAKE_ALPHA(0xFF);

			return sf::Color(TNGET_RED(col), TNGET_GREEN(col), TNGET_BLUE(col), TNGET_ALPHA(col));
		}
	}

	return sf::Color::White;
}

bool is_truestring(const string& str)
{
	return str == L"1" || str == L"true" || str == L"yes";
}

class PackageLoader
{
	typedef shared_ptr<sf::Texture> Texture;
	typedef shared_ptr<sf::Sprite> Sprite;
	typedef shared_ptr<vps::VirtualPackageSource> vpsp;
	typedef shared_ptr<StdTile> Tile;
	typedef rapidxml::xml_node<wchar_t> xml_node;

	class Raster
	{
	public:
		Raster(Texture tex, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
			: mPosition(x, y)
			, mSize(w, h)
			, mTex(tex)
		{
		}

		Tile makeSprite(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const sf::Color& color)
		{
			Point p = mPosition + Point(x, y) * mSize;
			Rect s = mSize * Point(std::max<unsigned int>(1, w), std::max<unsigned int>(1, h));
			auto sp = std::make_shared<StdSpriteTile>(sf::Sprite(*mTex, sf::IntRect(p.x, p.y, s.x, s.y)), mTex, color);

			return sp;
		}

	private:
		Texture mTex;
		Point mPosition;
		Rect mSize;
	};

	class Scope
	{
	public:
		Scope(PackageLoader& pl, const string& scope_name, std::vector<string>& scopes, vpsp src, xml_node& node)
			: pl(pl)
			, mScopes(scopes)
			, mSource(src)
			, mScopeNode(node)
			, log(pl.log)
			, mIsNewScope(scope_name.size())
		{
			addFunction(L"information", &Scope::informationDummy);
			addFunction(L"string", &Scope::defineString);
			addFunction(L"tile", &Scope::defineTile);
			addFunction(L"scope", &Scope::enterScopes);
			addFunction(L"image", &Scope::defineImage);
			addFunction(L"raster", &Scope::defineRaster);
			addFunction(L"aspect", &Scope::defineAspect);
			addFunction(L"animation", &Scope::defineAnimation);

			if(mIsNewScope)
				mScopes.push_back(scope_name);
		}

		~Scope()
		{
			if(mIsNewScope)
				mScopes.pop_back();
		}


		void process()
		{
			for(auto& child : rxml::children(mScopeNode))
			{
				auto name = rxml::name(child);
				auto it = mNodeOperators.find(name);

				if(it != mNodeOperators.end())
				{
					try {
						it->second(child);
					}catch(excp::ExceptionBase& e)
					{

						log.error() << L"Node illformed: " << e.why();

					}catch(boost::bad_lexical_cast&)
					{
						log.error() << L"Failed to parse numeric value in " << rxml::locate(child);
					
					}catch(...)
					{
						log.error() << L"Unknown error in " << rxml::locate(child);
					}
				}else{
					log.warn() << L"Unknown xml node [" << name << "] in " << rxml::locate(mScopeNode);
				}
			}
		}

	private:
		void informationDummy(xml_node&)
		{

		}

		void defineString(xml_node& node)
		{
			string name = resolveString(rxml::value(node, L":name"));
			string value = resolveString(rxml::valuefb(node, L":value", node.value()));

			addResource(pl.mStringResources, name, value, L"string", node);
		}

		void defineAspect(xml_node& node)
		{
			string name = resolveString(rxml::valuefb(node, L":name", L"std"));

			unsigned int w = boost::lexical_cast<unsigned int>(resolveString(rxml::value(node, L":w")));
			unsigned int h = boost::lexical_cast<unsigned int>(resolveString(rxml::value(node, L":h")));
			
			addResource(pl.mAspects, name, Rect(w, h), L"aspect", node);
		}

		void defineImage(xml_node& node)
		{
			string name = resolveString(rxml::value(node, L":name"));
			string src = resolveString(rxml::value(node, L":src"));

			std::vector<byte> rawimg = mSource->loadRaw(src);
			Texture tex = std::make_shared<sf::Texture>();
			tex->loadFromMemory(rawimg.data(), rawimg.size());

			addResource(pl.mImages, name, tex, L"image", node);
		}

		void defineRaster(xml_node& node)
		{
			string name = resolveString(rxml::value(node, L":name"));
			auto tex = resolveReference(pl.mImages, resolveString(rxml::value(node, L":img")));

			unsigned int x = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":x", L"0")), 0);
			unsigned int y = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":y", L"0")), 0);
			unsigned int w = boost::lexical_cast<unsigned int>(resolveString(rxml::valuefb(node, L":w", L"0")));
			unsigned int h = boost::lexical_cast<unsigned int>(resolveString(rxml::valuefb(node, L":h", L"0")));

			Raster r(tex, x, y, w, h);
			addResource(pl.mRasters, name, r, L"raster", node);
		}

		shared_ptr<StdTile> defineTileContent(xml_node& node, bool needname)
		{
			using boost::lexical_cast;
			const string name = resolveString(rxml::valuefb(node, L":name", L""));

			if(needname && name.empty())
			{
				BOOST_THROW_EXCEPTION(excp::FormatException() << excp::InfoWhat(L"Name attribute was empty in a tile definition"));
			}

			shared_ptr<StdTile> tile;
			rapidxml::xml_attribute<wchar_t>* attr;

			sf::Color tileColor = parseColorString(resolveString(rxml::valuefb(node, L":color", L"")));

			if(attr = node.first_attribute(L"img"))
			{
				auto tex = resolveReference(pl.mImages, resolveString(rxml::value(attr)));

				unsigned int x = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":x", L"0")), 0);
				unsigned int y = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":y", L"0")), 0);
				unsigned int w = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":w", L"0")), UINT_MAX);
				unsigned int h = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":h", L"0")), UINT_MAX);

				unsigned int texw = tex->getSize().x;
				unsigned int texh = tex->getSize().y;

				if(texw <= x)
					x = 0;

				if(texh <= y)
					y = 0;

				if(w + x >= texw)
					w = texw - x;

				if(h + y >= texh)
					h = texh - y;

				auto sprite = sf::Sprite(*tex, sf::IntRect(x, y, w, h));
				tile = std::make_shared<StdSpriteTile>(std::ref(sprite), tex, tileColor);

			}else if(attr = node.first_attribute(L"raster"))
			{
				auto raster = resolveReference(pl.mRasters, resolveString(rxml::value(attr)));

				unsigned int x = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":x", L"0")), 0);
				unsigned int y = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":y", L"0")), 0);
				unsigned int w = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":w", L"0")), 1);
				unsigned int h = cast_default<unsigned int>(resolveString(rxml::valuefb(node, L":h", L"0")), 1);

				tile = raster.makeSprite(x, y, w, h, tileColor);


			}else if(attr = node.first_attribute(L"std"))
			{
				tile = std::make_shared<StdRefTile>(StdTilePool::Inst().getStdTile(resolveString(rxml::value(attr))), tileColor);
			}

			if(name.size())
			{
				if(tile)
					addResource(pl.mTiles, name, tile, L"tile", node);
				else
					log.error() << L"Failed to create tile '" << name << L"' in " << rxml::locate(node);
			}

			return tile;
		}

		void defineTile(xml_node& node)
		{
			defineTileContent(node, true);
		}


		void defineAnimation(xml_node& node)
		{
			const string name = resolveString(rxml::value(node, L":name"));

			std::vector<shared_ptr<StdTile>> tiles;

			for(auto* tileNode = node.first_node(L"tile");
				tileNode;
				tileNode = tileNode->next_sibling())
			{
				auto tile = defineTileContent(*tileNode, false);
				tiles.push_back(tile);
			}

			std::vector<unsigned int> portions;
			string portionString = resolveString(rxml::valuefb(node, L":intp", L""));
			std::replace(portionString.begin(), portionString.end(), L',', L' ');

			std::wistringstream portionStream(portionString);
			std::copy(std::istream_iterator<int, wchar_t>(portionStream), std::istream_iterator<int, wchar_t>(), std::back_inserter(portions));
			


			const string random = resolveString(rxml::valuefb(node, L":rand", L"0"));
			unsigned int interval = boost::lexical_cast<unsigned int>(resolveString(rxml::value(node, L":int")));


			shared_ptr<StdTile> anim = std::make_shared<StdAnimationTile>(interval, std::move(tiles), std::move(portions), is_truestring(random));


			addResource(pl.mTiles, name, anim, L"animation", node);
		}

		void enterScopes(xml_node& node)
		{
			if(rxml::valuefb(node, L":ignore", L"") == L"true")
				return;

			Scope scope(pl, rxml::valuefb(node, L":name", L""), mScopes, mSource, node);
			scope.process();
		}

		string resolveString(string&& str)
		{
			if(str.size() && str.front() == L'@')
			{
				return resolveReference(pl.mStringResources, str.substr(1));
			}

			return str;
		}

		template<typename T>
		T resolveReference(const std::unordered_map<string, T>& storage, const string& fullref)
		{
			unsigned int sccount = mScopes.size();

			string relref;
			if(fullref.find(L"::") == 0)
			{
				sccount = 0;
				relref = fullref.substr(2);
			}else{
				relref = fullref;
			}

			do {
				string ref = buildScopeName(relref, sccount);

				auto it = storage.find(ref);

				if(it != storage.end())
					return it->second;

			}while(sccount--);

			BOOST_THROW_EXCEPTION(excp::FormatException() << excp::InfoWhat(L"Failed to resolve reference[" + fullref + L"]"));
		}

		string buildScopeName(const string& name, unsigned int deep = UINT_MAX)
		{
			if(name.find(L"::") == 0)
				return name.substr(2);

			if(!deep)
				return name;

			if(deep > mScopes.size())
				deep = mScopes.size();

			string result;
			for(unsigned int idx = 0; idx < deep; ++idx)
			{
				result += mScopes[idx] + L"::";
			}

			return result + name;
		}

		template<typename T>
		void addResource(std::unordered_map<string, T>& target, const string& name, const T& res, const string& res_name, const rapidxml::xml_base<wchar_t>& entity)
		{
			bool insterted;
			std::tie(std::ignore, insterted) = target.insert(std::make_pair(buildScopeName(name), res));

			if(!insterted)
				log.warn() << L"Redefinition of " << res_name << " '" << name << L"' in " << rxml::locate(entity);
		}

		template<typename F>
		void addFunction(const string& name , F func)
		{
			using namespace std::placeholders;

			bool inserted;
			std::tie(std::ignore, inserted) = TILENET_EMPLACE(mNodeOperators, name, std::bind(func, this, _1));
		}

	private:
		Log& log;
		PackageLoader& pl;
		const bool mIsNewScope;
		std::vector<string>& mScopes;
		vpsp mSource;
		xml_node& mScopeNode;

		std::unordered_map<string, std::function<void (xml_node&)>> mNodeOperators;
	};


	void extractPackageInfo(const rapidxml::xml_document<wchar_t>& doc)
	{
		auto& info_node = rxml::getnode(doc, L"package/information");


		mPInfo =	PackageInfo(rxml::value(info_node, L":name"),
								L"1.0.0.0",
								L"1.0.0.0",
								rxml::valuefb(info_node, L"author:name", L"unknown"),
								mRootSource->srcname(),
								std::vector<string>());
	}

	void parseXml(std::vector<wchar_t>& content, rapidxml::xml_document<wchar_t>& doc)
	{
		doc.parse<	rapidxml::parse_trim_whitespace |
					rapidxml::parse_validate_closing_tags |
					rapidxml::parse_normalize_whitespace>(content.data());
	}

	void open(vpsp src, const string& path, bool isRoot = false)
	{
		std::vector<string> scope_names;
		std::vector<wchar_t> txt = src->loadText(path);

		rapidxml::xml_document<wchar_t> doc;

		try {
			parseXml(txt, doc);
		}catch(rapidxml::parse_error& e)
		{
			log.error() << "Failed to parse " << path << "[" << e.what() << "]";
			return;
		}

		if(isRoot)
			extractPackageInfo(doc);

		auto& package_root = rxml::getnode(doc, L"package");

		Scope scope(*this, rxml::valuefb(package_root, L":scopename", L""), scope_names, src, package_root);
		scope.process();
	}


public:
	PackageLoader(const vpsp& rootSource)
		: mRootSource(rootSource)
		, log(L"ploader")
		, mPInfo(L"Dummy Package", L"no version", L"no version", L"no author", L"---", std::vector<string>())
	{
	}


	shared_ptr<Package> loadPackage()
	{
		log.info() << L"Loading package in " << mRootSource->srcname();
		open(mRootSource, L"package.xml", true);

		return std::make_shared<Package>(mPInfo, std::move(mTiles), std::move(mAspects));
	}

	PackageInfo loadPackageInfo()
	{
		std::vector<wchar_t> txt = mRootSource->loadText(L"package.xml");

		rapidxml::xml_document<wchar_t> doc;

		try {
			parseXml(txt, doc);
		}catch(rapidxml::parse_error& e)
		{
			log.error() << "Failed to parse package.xml[" << e.what() << "]";
			BOOST_THROW_EXCEPTION(excp::XmlException() << excp::InfoWhat(L"Failed to load package.xml!"));
		}

		extractPackageInfo(doc);

		return mPInfo;
	}



private:
	vpsp mRootSource;
	PackageInfo mPInfo;
	std::unordered_map<string, string> mStringResources;
	std::unordered_map<string, Texture> mImages;
	std::unordered_map<string, Tile> mTiles;
	std::unordered_map<string, Raster> mRasters;
	std::unordered_map<string, Rect> mAspects;

	std::unordered_set<string> mAlreadyOpened;


	Log log;
};




shared_ptr<Package> loadPackage( const fs::path& ppath )
{
	typedef shared_ptr<vps::VirtualPackageSource> vpsp;
	vpsp source = vps::LoadVPSFromSF(ppath);

	PackageLoader loader(source);

	return loader.loadPackage();
}

PackageInfo loadPackageInfo( const fs::path& ppath )
{
	typedef shared_ptr<vps::VirtualPackageSource> vpsp;
	vpsp source = vps::LoadVPSFromSF(ppath);

	PackageLoader loader(source);

	return loader.loadPackageInfo();
}



}}
