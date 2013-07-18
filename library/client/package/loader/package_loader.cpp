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

	




	void open(vpsp src, const string& path)
	{
		std::vector<wchar_t> txt = src->loadText(path);

		rapidxml::xml_document<wchar_t> doc;
		doc.parse<	rapidxml::parse_trim_whitespace |
					rapidxml::parse_validate_closing_tags |
					rapidxml::parse_normalize_whitespace>(txt.data());

		auto& package_root = rxml::getnode(doc, L"package");

		for(auto& child : rxml::children(package_root))
		{
			auto name = rxml::name(child);
			auto it = mNodeOperators.find(name);

			if(it != mNodeOperators.end())
				it->second(child);
			else
				log.warn() << L"Unknown xml node [" << name << "] in " << path << L"::" << rxml::locate(child);
		}

	}


public:
	PackageLoader(const vpsp& rootSource)
		: mRootSource(rootSource)
		, log(L"ploader")
	{
	}


	shared_ptr<Package> load()
	{
		open(mRootSource, L"package.xml");

		return nullptr;
	}



private:
	vpsp mRootSource;
	std::unordered_map<string, string> mStringResources;
	std::unordered_map<string, Texture> mImages;
	std::unordered_map<string, StdTile> mTiles;

	std::unordered_set<string> mAlreadyOpened;

	std::unordered_map<string, std::function<void (rapidxml::xml_node<wchar_t>&)>> mNodeOperators;

	Log log;
};




shared_ptr<Package> loadPackage( const fs::path& ppath )
{
	typedef shared_ptr<vps::VirtualPackageSource> vpsp;
	vpsp source = vps::LoadVPSFromSF(ppath);

	PackageLoader loader(source);

	return loader.load();
}



}}
