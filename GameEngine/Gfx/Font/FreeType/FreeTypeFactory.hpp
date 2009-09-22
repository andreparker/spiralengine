#ifndef FREE_TYPE_FACTORY_HPP
#define FREE_TYPE_FACTORY_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../FontFactory.hpp"

namespace Spiral
{

	class FreeTypeFactory : public FontFactory
	{
	public:
		FreeTypeFactory();
		~FreeTypeFactory();

	private:

		FT_Library m_library;
	private:
		virtual bool DoInitialize();
		virtual boost::shared_ptr< Font > DoCreateFont( const boost::shared_ptr< IFile >& fontFile, boost::int32_t charWidth, boost::int32_t charHeight );
	};
}

#endif