#include <boost/cstdint.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <gl/GL.h>
#include "../GfxImpl.hpp"
#include "WinOglDriver.hpp"
#include "WinOglException.hpp"
#include "../Ogl/OglGeometry.hpp"
#include "../GeometryType.hpp"
#include "../Ogl/OglUtility.hpp"
#include "../ogl/OglDriver.hpp"
#include "../VertexBuffer.hpp"
#include "../IndexBuffer.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;

WinOglDriver::WinOglDriver():
m_rc(0),m_dc(0),m_glDriver(NULL)
{
	m_glDriver = new OglDriver;
}

WinOglDriver::~WinOglDriver()
{
	DoUnInitialize();
	delete m_glDriver;
	m_glDriver = NULL;
}

bool WinOglDriver::DoInitialize( const boost::any& data )
{
    HDC dc = any_cast< HDC >( data );

    PIXELFORMATDESCRIPTOR pfd = { 
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
        1,                     // version number 
        PFD_DRAW_TO_WINDOW |   // support window 
        PFD_SUPPORT_OPENGL |   // support OpenGL 
        PFD_DOUBLEBUFFER,      // double buffered 
        PFD_TYPE_RGBA,         // RGBA type 
        24,                    // 24-bit color depth 
        0, 0, 0, 0, 0, 0,      // color bits ignored 
        0,                     // no alpha buffer 
        0,                     // shift bit ignored 
        0,                     // no accumulation buffer 
        0, 0, 0, 0,            // accum bits ignored 
        16,                    // 16-bit z-buffer     
        0,                     // no stencil buffer 
        0,                     // no auxiliary buffer 
        PFD_MAIN_PLANE,        // main layer 
        0,                     // reserved 
        0, 0, 0                // layer masks ignored 
    }; 

    int32_t pixelFormat = ChoosePixelFormat( dc, &pfd );

    if( pixelFormat )
    {
        SetPixelFormat( dc, pixelFormat, &pfd );
        CreateContext(dc);
        FillInDeviceInfo();
        
    }

    return bool(m_rc != NULL);
}

bool WinOglDriver::DoUnInitialize()
{
	if( m_rc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_rc );
		m_rc = NULL;
		return true;
	}

	return false;
}

void WinOglDriver::CreateContext( HDC dc )
{
    m_rc = wglCreateContext( dc );

    if( m_rc == NULL )
    {
        throw WinOglException( "Error creating render context!");
    }

    if( wglMakeCurrent( dc, m_rc ) == FALSE )
    {
        throw WinOglException( "Error making render context current!" );
    }

    m_dc = dc;          // save the dc for later use
}

void WinOglDriver::FillInDeviceInfo()
{
    SetName( string("GfxDriver: ") + reinterpret_cast<const char*>(glGetString( GL_RENDERER )) + "\n" );
    SetInfo( string("Vendor: ") + reinterpret_cast<const char*>(glGetString( GL_VENDOR )) + "\n" );
    SetInfo( GetInfo() + "Version: " + reinterpret_cast<const char*>(glGetString( GL_VERSION )) + "\n" );
    SetInfo( GetInfo() + "Extensions: " + reinterpret_cast<const char*>(glGetString( GL_EXTENSIONS )) + "\n" );
}

bool WinOglDriver::DoEnumerateSettings( std::list< GfxVidInfo_t >& modeList )
{
    DEVMODE devMode;
    DWORD modeNum = 0;      // 0 - n display modes
    GfxVidInfo_t info;

    // this returns false when it has exhausted all display modes
    while( EnumDisplaySettings( NULL, modeNum++, &devMode ) == TRUE )
    {
        info.width = devMode.dmPelsWidth;
        info.height = devMode.dmPelsHeight;
        info.bitDepth = devMode.dmBitsPerPel;

        modeList.push_back( info );
    }

	return bool( modeNum > 0 );
}

bool WinOglDriver::DoSetVideo( const GfxVidInfo_t& info, bool bfullscreen /*= true*/ )
{
    DWORD dwflags = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
    DEVMODE devmode;

    devmode.dmSize = sizeof( devmode );
	devmode.dmFields = dwflags;
    devmode.dmBitsPerPel = info.bitDepth;
    devmode.dmPelsWidth  = info.width;
    devmode.dmPelsHeight = info.height;

    LONG results = ChangeDisplaySettings( &devmode, bfullscreen ? CDS_FULLSCREEN : CDS_TEST );

    return bool(results == DISP_CHANGE_SUCCESSFUL);
}

bool WinOglDriver::DoBeginDraw()
{
	return m_glDriver->BeginDraw();
}

void WinOglDriver::DoEndDraw()
{
	m_glDriver->EndDraw();
}

bool WinOglDriver::DoPresent()
{
	bool result = m_glDriver->Present();

    if( m_dc == NULL )
    {
        throw WinOglException( "Invalid dc" );
    }

    return bool( SwapBuffers( m_dc ) == TRUE && result );
}

void WinOglDriver::DoBind( const boost::shared_ptr< Texture >& texture, boost::int32_t unit )
{
	m_glDriver->Bind( texture, unit );
}

bool WinOglDriver::DoCreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry )
{
	return m_glDriver->CreateGeometry( type, geometry );
}

void WinOglDriver::DoDraw( boost::shared_ptr<Geometry>& geometry )
{
	m_glDriver->Draw( geometry );
}

void WinOglDriver::DoSetState( const RenderState& state )
{
	m_glDriver->SetState( state );
}