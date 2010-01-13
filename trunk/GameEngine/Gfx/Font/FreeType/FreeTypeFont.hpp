#ifndef FREE_TYPE_FONT_HPP
#define FREE_TYPE_FONT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include "../../Font.hpp"


namespace Spiral
{

	class FreeTypeFont : public Font
	{
	public:
		virtual ~FreeTypeFont();
		FreeTypeFont( const FreeTypeFont& font );
		FreeTypeFont( FT_Library library, FT_Face font,const boost::shared_array< boost::int8_t >& data, boost::int32_t width, boost::int32_t height );

	private:
		virtual void DoCalcSurfaceSize( const wString& str, boost::int32_t& surfWidth, boost::int32_t& surfHeight );
		virtual void DoRenderAlpha( boost::shared_ptr< Surface >& surface, const wString& str, const Rgba& color );
		virtual void DoRenderOpaque( boost::shared_ptr< Surface >& surface, const wString& str, const Rgba& color );
		virtual void DoRenderAlpha( boost::shared_ptr< Surface >& surface, boost::uint32_t& cursorX, const wString& str, const Rgba& color );
		virtual void DoRenderOpaque( boost::shared_ptr< Surface >& surface, boost::uint32_t& cursorX, const wString& str, const Rgba& color );
		virtual bool DoSetSize( boost::int32_t width, boost::int32_t height );

		virtual FreeTypeFont* DoClone()const;
		void SetTheFontSize();
	private:
		FT_Library m_library;
		boost::shared_ptr< FT_FaceRec_ > m_face;
		boost::shared_array< boost::int8_t > m_data;		// holds the free type data (actual file...)
	};
}

#endif