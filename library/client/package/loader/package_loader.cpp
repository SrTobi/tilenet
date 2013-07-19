#include "includes.hpp"
#include "package_loader.hpp"

#include "client/package/std_tile_pool.hpp"
#include "client/package/components/std_tile.hpp"
#include "virtual_package_source.hpp"

#include "utils/xml_helpers.hpp"

#include <rxml/iterators.hpp>
#include <rxml/value.hpp>

namespace client {
namespace pload {


class PackageLoader
{
	typedef shared_ptr<sf::Texture> Texture;
	typedef shared_ptr<sf::Sprite> Sprite;
	typedef shared_ptr<vps::VirtualPackageSource> vpsp;
	typedef shared_ptr<StdTile> Tile;
	typedef rapidxml::xml_node<wchar_t> xml_node;

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
					it->second(child);
				else
					log.warn() << L"Unknown xml node [" << name << "] in " << rxml::locate(mScopeNode);
			}
		}

	private:
		void informationDummy(xml_node&)
		{

		}

		void defineString(xml_node& node)
		{
			const string name = rxml::value(node, L":name");
			string value = rxml::valuefb(node, L":value", node.value());

			addResource(pl.mStringResources, name, resolveString(std::move(value)), L"string", node);
		}

		void defineTile(xml_node& node)
		{
			const string name = rxml::value(node, L":name");
			shared_ptr<StdTile> tile;

			for(auto& attr : rxml::attributes(node))
			{
				const string def = rxml::name(attr);

				if(def == L"std")
				{
					tile = StdTilePool::Inst().getStdTile(resolveString(rxml::value(attr)));
				}
			}

			if(tile)
				addResource(pl.mTiles, name, tile, L"tile", node);
			else
				log.warn() << "Failed to load tile '" << name << L"' in " << rxml::locate(node);
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
				unsigned int sccount = mScopes.size();

				if(str.find(L"::") == 1)
				{
					sccount = 0;
					str = str.substr(3);
				}else{
					str = str.substr(1);
				}

				do {
					string ref = buildScopeName(str, sccount);

					auto it = pl.mStringResources.find(ref);

					if(it != pl.mStringResources.end())
						return it->second;

				}while(sccount--);

				NOT_IMPLEMENTED(/* failed to resolve ref */)

			}else{
				return str;
			}
		}

		string buildScopeName(const string& name, unsigned int deep = MAXUINT)
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
		parseXml(txt, doc);

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

		return std::make_shared<Package>(mPInfo, std::move(mTiles));
	}

	PackageInfo loadPackageInfo()
	{
		std::vector<wchar_t> txt = mRootSource->loadText(L"package.xml");

		rapidxml::xml_document<wchar_t> doc;
		parseXml(txt, doc);

		extractPackageInfo(doc);

		return mPInfo;
	}



private:
	vpsp mRootSource;
	PackageInfo mPInfo;
	std::unordered_map<string, string> mStringResources;
	std::unordered_map<string, Texture> mImages;
	std::unordered_map<string, Tile> mTiles;

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
