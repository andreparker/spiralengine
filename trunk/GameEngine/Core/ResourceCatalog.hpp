#ifndef RESOURCE_CATALOG_HPP
#define RESOURCE_CATALOG_HPP

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

namespace Spiral
{
	class Texture;
	struct ResourceCatalog
	{
		typedef std::map< std::string, boost::shared_ptr< Texture > >::iterator TextureCatalogItr;
		std::map< std::string, boost::shared_ptr< Texture > > m_textureCatalog; 
	};
}

#endif