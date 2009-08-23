#include <string>
#include "BaseWindow.h"
#include "../WindowException.hpp"

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
	if (pWnd)
	{
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
	RECT rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = 600;
	rect.bottom = 400;

	return Create(WS_OVERLAPPEDWINDOW | WS_VISIBLE, &rect);
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
			char outBuffer[128];
			FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, 0, GetLastError(), outBuffer, 128, NULL );

			throw WindowException( string( "Window error! :" ) + outBuffer );
		}else if( results == 0 )
		{
			// a quit message posted
			return true;
		}
	}else
	{
		results = PeekMessage( &msg, m_hwnd, msgFilterMin, msgFilterMax, PM_REMOVE );
		if( results != 0 )
		{
			if( msg.message == WM_QUIT )
			{
				return true;
			}
		}
	}

	// no quit message
	return false;
}