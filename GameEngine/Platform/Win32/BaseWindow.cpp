#include <string>
#include "BaseWindow.h"
#include "WindowException.hpp"

using namespace std;

BOOL BaseWindow::RegisterWindow()
{
    WNDCLASSEX wcx; 
 
    // Fill in the window class structure with default parameters 
 
    wcx.cbSize = sizeof(WNDCLASSEX);							// size of structure 
    wcx.style = CS_HREDRAW | CS_VREDRAW;						// redraw if size changes 
    wcx.lpfnWndProc = BaseWindow::stWinMsgHandler;				// points to window procedure 
    wcx.cbClsExtra = 0;											// no extra class memory 
    wcx.cbWndExtra = 0;											// no extra window memory 
    wcx.hInstance = m_hInstance;									// handle to instance 
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// predefined app. icon 
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);					// predefined arrow 
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// white background brush 
    wcx.lpszMenuName = NULL;									// name of menu resource 
    wcx.lpszClassName = "BaseWindow";							// name of window class 
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				// small class icon 
 
    // Register the window class. 
    return RegisterWindow(&wcx); 

}

BOOL BaseWindow::RegisterWindow(UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, 
									LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm)
{
    WNDCLASSEX wcx; 
 
    // Fill in the window class structure with default parameters 
 
    wcx.cbSize = sizeof(WNDCLASSEX);				// size of structure 
    wcx.style = style;								// redraw if size changes 
    wcx.lpfnWndProc = BaseWindow::stWinMsgHandler;	// points to window procedure 
    wcx.cbClsExtra = 0;								// no extra class memory 
    wcx.cbWndExtra = 0;								// no extra window memory 
    wcx.hInstance = m_hInstance;						// handle to instance 
    wcx.hIcon = hIcon;								// predefined app. icon 
    wcx.hCursor = hCursor;							// predefined arrow 
    wcx.hbrBackground = hbrBackground;				// white background brush 
    wcx.lpszMenuName = lpszMenuName;				// name of menu resource 
    wcx.lpszClassName = lpszClassName;				// name of window class 
    wcx.hIconSm = hIconSm;							// small class icon 
 
    // Register the window class. 
    return RegisterWindow(&wcx); 
}

BOOL BaseWindow::RegisterWindow(CONST WNDCLASSEX* wcx)
{
	// Register the window class. 
	m_szClassName = wcx->lpszClassName;

	return RegisterClassEx( wcx ) == 0 ? FALSE : TRUE;
}

/*
	You can not initialize the window class with a class method as the window 
	procedure unless it is a static method, so the class also needs a static 
	message handler that determines which instance of the class is the recipient 
	of the message and calls that instance's window procedure.

	See "http://www.gamedev.net/reference/articles/article1810.asp" for more info.
*/
LRESULT CALLBACK BaseWindow::stWinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BaseWindow* pWnd;

	if (uMsg == WM_NCCREATE)
	{
		// get the pointer to the window from lpCreateParams which was set in CreateWindow
		SetWindowLong(hwnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	// get the pointer to the window
	pWnd = GetObjectFromWindow(hwnd);

	// if we have the pointer, go to the message handler of the window
	// else, use DefWindowProc
	if (pWnd && uMsg != 28 )
	{
		// call a registered handler
		pWnd->CallHandler( uMsg, wParam, lParam );
		return pWnd->WinMsgHandler(hwnd, uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

BOOL BaseWindow::Create()
{ 
	// Create the window
	return Create(WS_OVERLAPPEDWINDOW | WS_VISIBLE);
}

BOOL BaseWindow::Create(DWORD dwStyles, RECT* rect)
{ 
	// Create the window
	
	// send the this pointer as the window creation parameter
	m_hwnd = CreateWindow(m_szClassName.c_str(), m_szWindowTitle.c_str(), dwStyles, rect->left, rect->top, 
		rect->right - rect->left, rect->bottom - rect->top, NULL, NULL, m_hInstance, 
		(void *)this);

   if( m_hwnd )
   {
       m_dc = GetDC( m_hwnd );
   }

	return (m_hwnd != NULL);
}

BOOL BaseWindow::Create( DWORD dwStyles )
{
	RECT rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = 600;
	rect.bottom = 400;

	return Create(dwStyles, &rect);
}

void BaseWindow::Show( bool bshow /* = true */ )
{
	if( m_hwnd == NULL )
	{
		throw WindowException( "Null Window Handle" );
	}

	UpdateWindow( m_hwnd );
	ShowWindow( m_hwnd, bshow ? SW_NORMAL : SW_HIDE );
}

bool BaseWindow::ProcessMessage( MSG& msg, UINT msgFilterMin, UINT msgFilterMax, bool wait )
{
	BOOL results;

	if( wait )
	{
		results = GetMessage( &msg, m_hwnd, msgFilterMin, msgFilterMax );
		if( results == -1 )
		{
			throw WindowException( string( "Window error! :" ) );
		}else if( results == 0 )
		{
			// a quit message posted
			return true;
		}else
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}else
	{
		results = PeekMessage( &msg, m_hwnd, msgFilterMin, msgFilterMax, PM_REMOVE );
		if( results != 0 )
		{
			if( msg.message == WM_QUIT )
			{
				return true;
			}else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
	}

	// no quit message
	return false;
}

void BaseWindow::RegisterHandler( UINT uMsg, const boost::function< void( WPARAM , LPARAM ) >& callBack )
{
	if( m_callbacks.find( uMsg ) == m_callbacks.end() )
	{
		m_callbacks[ uMsg ] = callBack;
	}
}

void BaseWindow::CallHandler( UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	CallbackItr callback = m_callbacks.find( uMsg );
	if( callback != m_callbacks.end() )
	{
		(*callback).second( wParam, lParam );
	}

}

void BaseWindow::ResizeWindow( RECT* rect, bool fullscreen )
{
	WINDOWINFO info;
	RECT save = *rect; // save it

	if( fullscreen )
	{
		SetWindowLong( m_hwnd, GWL_STYLE, WS_POPUP );
	}else
	{
		SetWindowLong( m_hwnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU );
	}

	GetWindowInfo( m_hwnd, &info );
	AdjustWindowRect( rect, info.dwStyle, false );

	// keep positions same and just add the calculated sizes
	LONG dx = save.left - rect->left;
	LONG dy = save.top  - rect->top;
	rect->left += dx;
	rect->top  += dy;
	rect->bottom += dy;
	rect->right += dx;

	MoveWindow( m_hwnd,rect->left, rect->top, rect->right, rect->bottom ,TRUE );
}

BaseWindow::~BaseWindow()
{
	SetWindowLong( m_hwnd, GWL_USERDATA, (long)(NULL) );
}