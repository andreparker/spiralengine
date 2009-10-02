#include "WindowsLogger.hpp"
#include "WindowException.hpp"

using namespace Spiral;

WindowsLogger::WindowsLogger( HWND mainWindow ):
m_recieverWindow(NULL),
m_senderWindow( mainWindow )
{
	m_recieverWindow = FindWindow( NULL, "Console" );
	if( !m_recieverWindow )
	{
		// greater than 32 means succesful... sigh microsoft, casting hresult to a int..
		if( (int)ShellExecute( NULL, "open", "Console.exe", NULL, "", SW_SHOW ) > 32 )
		{
			while( !(m_recieverWindow = FindWindow( NULL, "Console" )) );
		}else
		{
			throw WindowException( "Could not open Console!" );
		}
		
	}
}

void WindowsLogger::log( const std::string& msg ) const
{
	if( m_recieverWindow )
	{
		COPYDATASTRUCT data;

		data.dwData = 0;
		data.cbData = msg.length();
		data.lpData = reinterpret_cast<PVOID>( const_cast<char*>(msg.c_str()) );

		SendMessage( m_recieverWindow, WM_COPYDATA, reinterpret_cast<WPARAM>(m_senderWindow), reinterpret_cast<LPARAM>(&data) );
	}
}