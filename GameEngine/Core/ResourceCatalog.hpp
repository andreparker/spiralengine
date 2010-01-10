#ifndef RESOURCE_CATALOG_HPP
#define RESOURCE_CATALOG_HPP

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/cstdint.hpp>

namespace Spiral
{
	class Texture;
	class Font;
	struct ResourceCatalog
	{
		typedef std::map< std::string, boost::shared_ptr< Texture > >::iterator TextureCatalogItr;
		typedef std::map< std::string, boost::shared_ptr< Font > >::iterator FontCatalogItr;

		ResourceCatalog():m_textureCatalog(),m_fontCatalog(),m_textureSize(0),m_fontSize(0){}
		std::map< std::string, boost::shared_ptr< Texture > > m_textureCatalog;
		std::map< std::string, boost::shared_ptr< Font > > m_fontCatalog;
		boost::uint32_t m_textureSize;
		boost::uint32_t m_fontSize;

	};
}

#endif