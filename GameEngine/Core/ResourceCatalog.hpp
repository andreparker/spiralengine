#ifndef RESOURCE_CATALOG_HPP
#define RESOURCE_CATALOG_HPP

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

namespace Spiral
{
	class Texture;
	class Font;
	struct ResourceCatalog
	{
		typedef std::map< std::string, boost::shared_ptr< Texture > >::iterator TextureCatalogItr;
		typedef std::map< std::string, boost::shared_ptr< Font > >::iterator FontCatalogItr;

		std::map< std::string, boost::shared_ptr< Texture > > m_textureCatalog;
		std::map< std::string, boost::shared_ptr< Font > > m_fontCatalog;
	};
}

#endif