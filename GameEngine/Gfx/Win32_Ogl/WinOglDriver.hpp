/*!
*/
#ifndef WIN_OGL_DRIVER_HPP
#define WIN_OGL_DRIVER_HPP

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "../GfxDriver.hpp"

namespace Spiral
{

	class OglDriver;
    class WinOglDriver : public GfxDriver
    {
	public:
		WinOglDriver();
		~WinOglDriver();

	private:
        virtual bool DoInitialize( const boost::any& data );
        virtual bool DoUnInitialize();

		virtual void DoSetState( const RenderState& state );
		virtual void DoDraw( boost::shared_ptr<Geometry>& geometry );
		virtual bool DoCreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry );
		virtual void DoBind( const boost::shared_ptr< Texture >& texture, boost::int32_t unit );
		virtual bool DoCreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::uint8_t* data );
        virtual bool DoSetVideo( const GfxVidInfo_t& info, bool bfullscreen );
        virtual bool DoEnumerateSettings( std::list< GfxVidInfo_t >& modeList );
        virtual bool DoBeginDraw();
        virtual void DoEndDraw();
        virtual bool DoPresent();
    private:
        void CreateContext( HDC dc );
        void FillInDeviceInfo();

        HGLRC m_rc;
        HDC   m_dc;
		OglDriver* m_glDriver;
    };
}

#endif