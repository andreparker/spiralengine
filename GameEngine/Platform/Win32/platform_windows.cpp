#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE

#include "platform_windows.hpp"
#include "../../Core/MemoryOperators.hpp"

#include <string>
#include <exception>
#include <iostream>
#include <wchar.h>
#include <boost/scope_exit.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/weak_ptr.hpp>



#include "WindowException.hpp"
#include "../../Gfx/Win32_Ogl/WinOglDriver.hpp"
#include "../../Audio/oal/OAL_AudioDriver.hpp"

#include "../../Core/Engine.hpp"
#include "../../Core/MemoryManager.hpp"
#include "../../Core/MemoryPolicyMalloc.hpp"
#include "../../Core/FileManager.hpp"
#include "../../Core/File.hpp"
#include "../../Core/MouseEvent.hpp"
#include "../../Math/Math.hpp"
#include "../../Core/GameObjectCreator.hpp"
#include "../../Gfx/VisualObjectCreator.hpp"

#include "../../Editor/Editor.hpp"

#include <shellapi.h>

using namespace Spiral;
using namespace boost;
using namespace std;

const std::string module = "^yPlatform :";

boost::shared_ptr< GfxDriver > g_gfxDriver;
boost::shared_ptr< Audio::AudioDriver > g_audioDriver;

list< shared_array<char> > g_argList;
int g_argc = 0;
bool g_keys[256] = {false};
boost::weak_ptr< Engine > g_engine;

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

void InitializeWindow( boost::shared_ptr< Engine >& engine, boost::shared_ptr< AppWindow >& appWindow )
{
	RECT rt;
	GfxVidInfo_t info;
	//shared_ptr< CVar > variables = engine->GetVariables();

	LOG_I( module + "^w - Window Attributes - \n" );
	LONG width = engine->GetVariable<int32_t>( "vid_width" );//variables->GetVarValue( "vid_width", EmptyType<LONG>() );
	LONG height = engine->GetVariable<int32_t>( "vid_height" );//variables->GetVarValue( "vid_height", EmptyType<LONG>() );
	int32_t fullscreen = engine->GetVariable<int32_t>( "vid_fullscreen" );//variables->GetVarValue( "vid_fullscreen", EmptyType<int32_t>() );

	LOG_I( module + "^w Width - %1% \n", width );
	LOG_I( module + "^w Height - %1% \n", height );
	LOG_I( module + "^w Fullscreen - %1% \n", fullscreen );
	LOG_I( module + "^w -         End       - \n" );


	rt.left = 0;
	rt.top = 0;
	rt.right = width;
	rt.bottom = height;

	info.width = width;
	info.height = height;
	info.bitDepth = 32;

	LOG_I( module + "^w setting window attributes....\n" );
	bool fs = (fullscreen==1) ? true : false;
	engine->GetGfxDriver()->SetVideo( info, fs ); 
	appWindow->ResizeWindow( &rt,  fs );
	appWindow->Show();
}

void SwapCurrentRunningApp( boost::shared_ptr<Application>& currentApp, boost::shared_ptr<Application>& newRunApp, const boost::any& data )
{
	boost::shared_ptr< Engine > engine( g_engine );

	if( engine )
	{
		currentApp->UnInit();
		engine->UnInitialize();

		engine->Initialize( g_gfxDriver, g_audioDriver, data );
		engine->LoadConfig( "Data/Config/Config.cfg" );
		newRunApp->Init( g_argc, g_argList, engine );
		
		swap( currentApp, newRunApp );
	}

}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE /*hPrevInstance*/,
					 LPTSTR    /*lpCmdLine*/,
					 int       /*nCmdShow*/)
{

	RegisterExceptionHandler();

	try
	{
		
		g_argc = ParseCommandLine( g_argList );
		boost::shared_ptr< Application > spGameApp = CreateApp();
		boost::shared_ptr< Application > spEditorApp( new Editor::App );
		boost::shared_ptr< AppWindow > spAppWindow( new AppWindow( hInstance ) );
		boost::shared_ptr< Engine > engine = Engine::Create();
		boost::shared_ptr< LogModule > winLogger;
		boost::any windowData;

		// get a weak pointer to engine
		g_engine = engine;

		if( spAppWindow )
		{
			if( !spAppWindow->RegisterWindow() )
			{
				THROW_WINDOW_EXCEPTION( "Could not register window" );
			}

			if( !spAppWindow->Create( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU ) )
			{
				THROW_WINDOW_EXCEPTION( "Could not create window" );
			}

			winLogger = boost::make_shared< WindowsLogger >( spAppWindow->Get( EmptyType<HWND>() ) );
			LOG_ADD_LOGGER( winLogger );

			g_gfxDriver = boost::make_shared< WinOglDriver >();
			g_audioDriver = boost::make_shared< Audio::OalAudioDriver >();

			windowData = boost::any( spAppWindow->Get( EmptyType<HDC>() ) );

			engine->Initialize( g_gfxDriver , g_audioDriver, windowData );
			
			if( FileManager::instance().openPack( "Data.zip" ) )
			{
				LOG_I( module + " ^rAll resource data being read from %1% file.\n", "Data.zip" );
			}else
			{
				LOG_I( module + " ^rAll resource data being read from file io.\n" );
			}

			/*//------------------------------------------------------------------------
			boost::shared_ptr< IFile > layoutTest;
			FileManager::instance().getFile( "Data/Config/window.layout", layoutTest );
			engine->GetGuiManager()->LoadLayout( layoutTest );
			//*/

			engine->LoadConfig( "Data/Config/Config.cfg" );

			LOG_I( module + " ^wInitializing MainWindow....\n" );
			spAppWindow->SetEventPublisher( engine->GetEventPublisher() );
			spAppWindow->Initialize();

			InitializeWindow( engine, spAppWindow );

			LOG_I( module + " ^wInitializing Application....\n" );
			spGameApp->Init( g_argc, g_argList, engine );

			LOG_I( module + " ^wInitialization Complete.\n" );

			LOG_D( "^y========================================\n" );
			LOG_D( "^yListing Game Objects...\n" );
			GameObjectCreator::instance().LogClassVarInfo();
			LOG_D( "^yListing Visual Objects...\n" );
			VisualObjectCreator::instance().LogClassVarInfo();
			LOG_D( "^y========================================\n" );
		}

		MSG msg;
		DWORD tickCount = GetTickCount();
		SpReal fracSecs = 0.015f;
		SpReal appChangeDelay = 0.0f;

		boost::shared_ptr< Application > currentAppRunning = spGameApp;
		boost::shared_ptr< Application > nextAppRunning = spEditorApp;

		// returns true if WM_QUIT is encountered
		while( false == spAppWindow->ProcessMessage( msg, 0, 0 ) &&
			   false == spAppWindow->Quit() )
		{
			if( currentAppRunning->Run( fracSecs, engine ) )
			{
				engine->Tick( fracSecs );
			}else
			{
				break;
			}
			
			if( g_keys[ VK_F2 ] == true && appChangeDelay >= 1.0f )
			{
				SwapCurrentRunningApp( currentAppRunning, nextAppRunning, windowData );
				appChangeDelay = 0.0f;
			}else if ( g_keys[ VK_F3 ] == true && appChangeDelay >= 1.0f )
			{
				SwapCurrentRunningApp( currentAppRunning, currentAppRunning, windowData );
				appChangeDelay = 0.0f;
			}

			fracSecs = GetSeconds< SpReal >( GetTickCount() - tickCount );
			appChangeDelay += fracSecs;
			tickCount = GetTickCount();
		}

		LOG_I( module + "^w Shutting down...\n" );
		currentAppRunning->UnInit();
		engine->UnInitialize();
		FileManager::instance().closePack();
	}
	catch ( std::exception& error )
	{
		MsgBoxError( error.what() );
	}
	catch ( ... )
	{
		MsgBoxError( "Unhandled exception!" );
	}

	boost::shared_ptr<OFile> ofile;
	FileManager::instance().createFile("MemoryAnalysis.log", ofile );
	MemoryManager< MallocPolicy, 0 >::instance().WriteAnalysis( ofile );
	ofile->Close();

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
	RegisterHandler( WM_LBUTTONDOWN, boost::bind( &AppWindow::LeftMouseDownCallBack, this, _1, _2 ) );
	RegisterHandler( WM_LBUTTONUP, boost::bind( &AppWindow::LeftMouseUpCallBack, this, _1, _2 ) );
	RegisterHandler( WM_MOUSEMOVE, boost::bind( &AppWindow::MouseMoveCallBack, this, _1, _2 ) );
	RegisterHandler( WM_CHAR, boost::bind( &AppWindow::CharInputCallBack, this, _1, _2 ) );
}

// variables used to pass data a long to events
// they are used to avoid many allocations that boost::any does
boost::any charInputData,mouseMoveData,
           mouseLUpData,mouseLDownData,
		   keyUpData,keyDownData;

void AppWindow::Initialize()
{
	m_sysAppEventSubscriber = boost::make_shared< EventSubscriber >( Event( Event::EVENT_ANY, Catagory_App_Status::value ) );
	m_eventPublisher->AddSubscriber( m_sysAppEventSubscriber );
	m_sysAppEventSubscriber->AddCallback( boost::bind( &AppWindow::SysAppEvents, this, _1, _2 ) );
}

void ScreenToWorld( int& x, int& y )
{
	if( !g_engine.expired() )
	{
		boost::shared_ptr<Engine> ptr = g_engine.lock();
		Math::Vector2f pos = Math::make_vector<SpReal>( static_cast<SpReal>(x), static_cast<SpReal>(y) );
		ptr->ScreenToWorld( pos, pos );
		x = static_cast<int>(pos[0]);
		y = static_cast<int>(pos[1]);
	}
}

void AppWindow::SysAppEvents( const Spiral::Event& event, const boost::any& /*data*/ )
{
	if( event.m_catagory.m_bits.to_ulong() == Catagory_App_Status::value &&
		event.m_eventId == event_AppStatus_shutdown )
	{
		// quit the app
		m_quit = true;
		DestroyWindow( m_hwnd );
	}
}

void AppWindow::KeyUpCallback( WPARAM wParam, LPARAM /*lParam*/ )
{
	if( m_eventPublisher )
	{
		keyUpData = int32_t(wParam);
		g_keys[ wParam ] = false;
		m_eventPublisher->Publish( Event( event_keyboard_key, Catagory_KeyBoard_KeyUp::value ), keyUpData );
	}
}

void AppWindow::KeyDownCallback( WPARAM wParam, LPARAM /*lParam*/ )
{
	if( m_eventPublisher )
	{
		keyDownData = int32_t(wParam);
		g_keys[ wParam ] = true;
		m_eventPublisher->Publish( Event( event_keyboard_key, Catagory_KeyBoard_KeyDown::value ), keyDownData );
	}
}

void AppWindow::LeftMouseDownCallBack( WPARAM wParam, LPARAM lParam )
{
	if( m_eventPublisher )
	{
		int x,y;
		GetMousePosLPARAM( x, y, lParam );
		ScreenToWorld( x, y );
		mouseLDownData = MouseEvent( MouseEvent::mouse1, x, y );
		m_eventPublisher->Publish( Event( event_mouse, Catagory_Mouse_MouseDown::value ), mouseLDownData  );
	}
}

void AppWindow::LeftMouseUpCallBack( WPARAM wParam, LPARAM lParam )
{
	if( m_eventPublisher )
	{
		int x,y;
		GetMousePosLPARAM( x, y, lParam );
		ScreenToWorld( x, y );
		mouseLUpData = MouseEvent( MouseEvent::mouse1, x, y );
		m_eventPublisher->Publish( Event( event_mouse, Catagory_Mouse_Up::value ), mouseLUpData  );
	}
}

void AppWindow::MouseMoveCallBack( WPARAM wParam, LPARAM lParam )
{
	if( m_eventPublisher )
	{
		int x,y;
		GetMousePosLPARAM( x, y, lParam );
		ScreenToWorld( x, y );
		mouseMoveData = MouseEvent( MouseEvent::mouse1, x, y );
		m_eventPublisher->Publish( Event( event_mouse, Catagory_Mouse_Move::value ), mouseMoveData  );
	}
}

void AppWindow::CharInputCallBack( WPARAM wParam, LPARAM /*lParam*/ )
{
	if( m_eventPublisher )
	{
		charInputData = uint32_t( wParam );
		m_eventPublisher->Publish( Event( event_keyboard_char, Catagory_KeyBoard_Char::value ), charInputData );
	}
}