#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE

#include <string>
#include <exception>
#include <iostream>
#include <wchar.h>
#include <boost/scope_exit.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>


#include "platform_windows.hpp"
#include "WindowException.hpp"
#include "../Gfx/Win32_Ogl/WinOglDriver.hpp"
#include "../Core/Engine.hpp"
#include "../Core/CVar.hpp"
#include <shellapi.h>

using namespace Spiral;
using namespace boost;
using namespace std;



/*!
   @function  GetSeconds
   @brief     converts miliseconds to seconds
   @return    T
   @param     DWORD miliSeconds - miliSeconds 1000 = 1 sec
*/
template< class T >
inline T GetSeconds( DWORD miliSeconds )
{
	return static_cast< T >( miliSeconds ) / 1000.0f;
}


/*!
   @function  MsgBoxError
   @brief     displays a message box for a error
   @return    void
   @param     const string & errorStr
*/
void MsgBoxError( const string& errorStr )
{
	string finalText = string( "Error thrown: " ) + errorStr + '\n';
	std::cerr << finalText.c_str();

	MessageBox( NULL, finalText.c_str(), "SpiralEngine", MB_OK );
}

/*!
   @function  ParseCommandLine
   @brief     parses the commandline params passed to the program
   @return    int - argument count
   @param     list< shared_array< char > > & arglist - a list of arguments
*/
int ParseCommandLine( list< shared_array< char > >& arglist )
{
	int argc;
	LPWSTR* argStrArray = CommandLineToArgvW( GetCommandLineW(), &argc );

	BOOST_SCOPE_EXIT( (argStrArray) )
	{
		// free memory allocated from CommandLineToArgvW
		if( NULL != argStrArray )
		{
			LocalFree( argStrArray );
		}
	}BOOST_SCOPE_EXIT_END;

	for( int32_t i = 0; i < argc; ++i )
	{
		int32_t strLen = wcslen( argStrArray[i] );
		shared_array< char > str( new char[strLen+1] );
		WideCharToMultiByte( CP_ACP, WC_DEFAULTCHAR, argStrArray[i], -1, str.get(), strLen+1, NULL, NULL );
		arglist.push_back( str );
	}

	return argc;
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE /*hPrevInstance*/,
					 LPTSTR    /*lpCmdLine*/,
					 int       /*nCmdShow*/)
{

	try
	{
		list< shared_array<char> > argList;
		int argc = ParseCommandLine( argList );
		shared_ptr< Application > spApp = CreateApp();
		shared_ptr< AppWindow > spAppWindow( new AppWindow( hInstance ) );
		shared_ptr< Engine > engine = Engine::Create();

		if( spAppWindow )
		{
			if( !spAppWindow->RegisterWindow() )
			{
				throw WindowException( "Could not register window" );
			}

			if( !spAppWindow->Create( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU ) )
			{
				throw WindowException( "Could not create window" );
			}

			shared_ptr< GfxDriver > gfxDriver = make_shared< WinOglDriver >();
			any data = any( spAppWindow->Get( EmptyType<HDC>() ) );
			engine->Initialize( gfxDriver , data );
			spApp->Init( argc, argList, engine );

			spAppWindow->SetEventPublisher( engine->GetEventPublisher() );
			spAppWindow->Initialize();

			RECT rt;
			shared_ptr< CVar > variables = engine->GetVariables();

			LONG width = variables->GetVarValue( "vid_width", EmptyType<LONG>() );
			LONG height = variables->GetVarValue( "vid_height", EmptyType<LONG>() );
			int32_t fullscreen = variables->GetVarValue( "vid_fullscreen", EmptyType<int32_t>() );

			rt.left = 0;
			rt.top = 0;
			rt.right = width;
			rt.bottom = height;

			spAppWindow->ResizeWindow( &rt, (fullscreen==1) ? true : false );
			spAppWindow->Show();
		}

		MSG msg;
		DWORD tickCount = GetTickCount();
		SpReal fracSecs = 0.015f;

		// returns true if WM_QUIT is encountered
		while( false == spAppWindow->ProcessMessage( msg, 0, 0 ) &&
			   false == spAppWindow->Quit() )
		{
			spApp->Run( fracSecs, engine );
			engine->Tick( fracSecs );

			fracSecs = GetSeconds< SpReal >( GetTickCount() - tickCount );
			tickCount = GetTickCount();
		}

		spApp->UnInit();
		engine->UnInitialize();
	}
	catch ( std::exception& error )
	{
		MsgBoxError( error.what() );
	}
	catch ( ... )
	{
		MsgBoxError( "Unhandled exception!" );
	}

	return 0;
}


LRESULT CALLBACK AppWindow::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( uMsg )
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return TRUE;
	default:
		break;
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

AppWindow::AppWindow( HINSTANCE hInst, CONST WNDCLASSEX* wcx /* = NULL */ ):
BaseWindow( hInst, wcx ),
m_eventPublisher(),
m_sysAppEventSubscriber(),
m_quit(false)
{
	RegisterHandler( WM_KEYUP, boost::bind( &AppWindow::KeyUpCallback, this, _1, _2 ) );
	RegisterHandler( WM_KEYDOWN, boost::bind( &AppWindow::KeyDownCallback, this, _1, _2 ) );
}

void AppWindow::KeyUpCallback( WPARAM wParam, LPARAM lParam )
{
	if( m_eventPublisher )
	{
		m_eventPublisher->Publish( Event( event_keyboard_key, Catagory_KeyBoard_KeyUp::value ), any( int32_t(wParam) ) );
	}
}

void AppWindow::KeyDownCallback( WPARAM wParam, LPARAM lParam )
{
	if( m_eventPublisher )
	{
		m_eventPublisher->Publish( Event( event_keyboard_key, Catagory_KeyBoard_KeyDown::value ), any( int32_t(wParam) ) );
	}
}

void AppWindow::Initialize()
{
	m_sysAppEventSubscriber = make_shared< EventSubscriber >( Event( Event::EVENT_ANY, Catagory_App_Status::value ) );
	m_eventPublisher->AddSubscriber( m_sysAppEventSubscriber );
	m_sysAppEventSubscriber->AddCallback( boost::bind( &AppWindow::SysAppEvents, this, _1, _2 ) );
}

void AppWindow::SysAppEvents( const Spiral::Event& event, const boost::any& data )
{
	if( event.m_catagory.m_bits.to_ulong() == Catagory_App_Status::value &&
		event.m_eventId == event_AppStatus_shutdown )
	{
		// quit the app
		m_quit = true;
		DestroyWindow( m_hwnd );
	}
}