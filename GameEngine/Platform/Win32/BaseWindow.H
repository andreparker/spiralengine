// here is the base class for my windows

#ifndef _BASEWINDOW_H_
#define _BASEWINDOW_H_

#define WIN32_LEAN_AND_MEAN

#include <map>
#include <string>
#include <Windows.h>
#include <boost/function.hpp>

#include "../../Core/TypeUtils.hpp"

class BaseWindow
{
public:
	void RegisterHandler( UINT uMsg, boost::function< void( WPARAM , LPARAM ) > callBack );
	// many different ways to register
	virtual BOOL RegisterWindow();
	virtual BOOL RegisterWindow(UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, 
		LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm);
	virtual BOOL RegisterWindow(CONST WNDCLASSEX* wcx);

	// static message handler to put in WNDCLASSEX structure
	static LRESULT CALLBACK stWinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// just so you can change the window caption...
	void SetWindowTitle(LPCTSTR lpszTitle) 
	{
		m_szWindowTitle = lpszTitle;
	};

	// 2 ways to create
	virtual BOOL Create();
	virtual BOOL Create(DWORD dwStyles);
	virtual BOOL Create(DWORD dwStyles, RECT* rect);

	void ResizeWindow( RECT* rect, bool fullscreen = false );

	void Show( bool bshow = true );

	bool ProcessMessage( MSG& msg, UINT msgFilterMin, UINT msgFilterMax, bool wait = false );

	BOOL IsWindowClosed() 
	{ 
		return m_bWindowClosed; 
	};

   template< class T >
   T& Get( Spiral::EmptyType<T> );

   template<> HDC& Get( Spiral::EmptyType<HDC> );
   template<> HWND& Get( Spiral::EmptyType<HWND> );

   operator HDC()const
   {
       return m_dc;
   }

	operator HWND()const
	{
		return m_hwnd;
	}
public:

	void GetMousePosLPARAM( int& x,int& y, LPARAM lParam )const
	{
		x = lParam & 0x0000FFFF;
		y = ( lParam >> 16 ) & 0x0000FFFF;
	}

private:
		void CallHandler( UINT uMsg, WPARAM wParam, LPARAM lParam );
protected:
	HINSTANCE m_hInstance;
	HWND m_hwnd;
	HDC m_dc;
	BOOL m_bWindowClosed;
	std::string m_szClassName;
	std::string m_szWindowTitle;
	typedef std::map< UINT, boost::function< void( WPARAM , LPARAM ) > >::iterator CallbackItr;
	std::map< UINT, boost::function< void( WPARAM , LPARAM ) > > m_callbacks;

	//constructor 
	BaseWindow(HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) 
		:m_hwnd(NULL),
      m_dc(NULL),
		m_hInstance(NULL),
		m_bWindowClosed(FALSE)
	{ 
		m_hInstance = hInst; 
		if (wcx != NULL) 
		{
			RegisterWindow(wcx);
		}
	};

	// the real message handler
	virtual LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	// returns a pointer the window (stored as the WindowLong)
	inline static BaseWindow *GetObjectFromWindow(HWND hWnd)
	{
		return reinterpret_cast< BaseWindow * >( GetWindowLong(hWnd, GWL_USERDATA) );
	}
};

template<> inline HDC& BaseWindow::Get( Spiral::EmptyType<HDC> )
{
    return m_dc;
}

template<> inline HWND& BaseWindow::Get( Spiral::EmptyType<HWND> )
{
    return m_hwnd;
}

#endif