#include "includes.hpp"
#include "package_loader.hpp"

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


	void defineString(std::vector<string>& scope, vpsp src, rapidxml::xml_node<wchar_t>& node)
	{
		const string name = rxml::value(node, L":name");
		string value = rxml::valuefb(node, L":name", node.value());
		
		bool insterted;
		std::tie(std::ignore, insterted) = TILENET_EMPLACE(mStringResources, name, value);

		if(!insterted)
			log.warn() << L"Redefinition of string '" << name << L"' in " << rxml::locate(node);
	}
	

	void doScope(std::vector<string>& scope, vpsp src, rapidxml::xml_node<wchar_t>& node)
	{
		const string scope_name = rxml::valuefb(node, L":scopename", L"");
		if(scope_name.size())
			scope.push_back(scope_name);

		for(auto& child : rxml::children(node))
		{
			auto name = rxml::name(child);
			auto it = mNodeOperators.find(name);

			if(it != mNodeOperators.end())
				it->second(std::ref(scope), src, child);
			else
				log.warn() << L"Unknown xml node [" << name << "] in " << rxml::locate(child);
		}

		if(scope_name.size())
			scope.pop_back();
	}


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
		std::vector<string> root_scope;
		std::vector<wchar_t> txt = src->loadText(path);

		rapidxml::xml_document<wchar_t> doc;
		parseXml(txt, doc);

		if(isRoot)
			extractPackageInfo(doc);

		auto& package_root = rxml::getnode(doc, L"package");

		doScope(root_scope, src, package_root);
	}

	template<typename F>
	void addFunction(const string& name , F func)
	{
		using namespace std::placeholders;

		bool inserted;
		std::tie(std::ignore, inserted) = TILENET_EMPLACE(mNodeOperators, name, std::bind(func, this, _1, _2, _3));
	}

public:
	PackageLoader(const vpsp& rootSource)
		: mRootSource(rootSource)
		, log(L"ploader")
		, mPInfo(L"Dummy Package", L"no version", L"no version", L"no author", L"---", std::vector<string>())
	{

		addFunction(L"string", &PackageLoader::defineString);
	}


	shared_ptr<Package> loadPackage()
	{
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

	std::unordered_map<string, std::function<void (std::vector<string>&, vpsp, rapidxml::xml_node<wchar_t>&)>> mNodeOperators;

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
