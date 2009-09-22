#ifndef FONT_HPP
#define FONT_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>

namespace Spiral
{

	class Rgba;
	class Surface;

	class Font : boost::noncopyable
	{
	public:
		virtual ~Font();

		/*!
		   @function  CalcSurfaceSize
		   @brief     calculates the min size of the surface to render a string to
		   @return    void
		   @param     const std::string & str
		   @param     boost::int32_t & surfWidth - width in pixels
		   @param     boost::int32_t & surfHeight - ...
		*/
		void CalcSurfaceSize( const std::string& str, boost::int32_t& surfWidth, boost::int32_t& surfHeight );

		/*!
		   @function  RenderAlpha
		   @brief     draws text to a surface with alpha
		   @return    void
		   @param     boost::shared_ptr< Surface > & surface - surface to draw to
		   @param     const std::string & str - text to draw
		   @param     const Rgba & color - color of the text
		*/
		void RenderAlpha( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color );

		/*!
		   @function  RenderOpaque
		   @brief     draws text to a surface with no alpha, does not modify alpha if present
		   @return    void
		   @param     boost::shared_ptr< Surface > & surface - surface to draw to
		   @param     const std::string & str - text to draw
		   @param     const Rgba & color - color of the text
		*/
		void RenderOpaque( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color );
		
		/*!
		   @function  GetCharWidth
		   @brief     returns the width in pixels
		   @return    boost::int32_t
		*/
		boost::int32_t GetCharWidth()const
		{
			return m_charWidth;
		}

		/*!
		   @function  GetCharHeight
		   @brief     returns the height in pixels
		   @return    boost::int32_t
		*/
		boost::int32_t GetCharHeight()const
		{
			return m_charHeight;
		}
	protected:
		Font( boost::int32_t width, boost::int32_t height );

	private:
		boost::int32_t m_charWidth,m_charHeight;

		virtual void DoCalcSurfaceSize( const std::string& str, boost::int32_t& surfWidth, boost::int32_t& surfHeight ) = 0;
		virtual void DoRenderAlpha( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color ) = 0;
		virtual void DoRenderOpaque( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color ) = 0;
	};
}

#endif