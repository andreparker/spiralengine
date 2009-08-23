#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE

#include <string>
#include <exception>
#include <iostream>
#include <wchar.h>
#include <boost/scope_exit.hpp>
#include <boost/make_shared.hpp>


#include "platform_windows.hpp"
#include "WindowException.hpp"
#include "../Gfx/Win32_Ogl/WinOglDriver.hpp"
#include "../Core/Engine.hpp"
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

int APIENTRY _tWinMain(HINSTANCE hInstance,
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

			if( !spAppWindow->Create() )
			{
				throw WindowException( "Could not create window" );
			}

			shared_ptr< GfxDriver > gfxDriver = make_shared< WinOglDriver >();
			any data = any( spAppWindow->Get( EmptyType<HDC>() ) );
			engine->Initialize( gfxDriver , data );
			spApp->Init( argc, argList, engine );
			spAppWindow->Show();
		}

		MSG msg;
		DWORD tickCount = GetTickCount();
		real fracSecs = 0.0f;
		while( false == spAppWindow->ProcessMessage( msg, 0, 0 ) )
		{
			spApp->Run( fracSecs, engine );
			engine->Tick( fracSecs );

			fracSecs = GetSeconds< real >( tickCount - GetTickCount() );
			tickCount = GetTickCount();
		}
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

AppWindow::AppWindow( HINSTANCE hInst, CONST WNDCLASSEX* wcx /* = NULL */ ):
BaseWindow( hInst, wcx )
{

}

LRESULT CALLBACK AppWindow::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}