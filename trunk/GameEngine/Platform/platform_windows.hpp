#ifndef PLATFORM_WINDOWS_HPP
#define PLATFORM_WINDOWS_HPP

#include "../Core/Application.hpp"
#include "Win32/BaseWindow.h"

namespace Spiral
{
	boost::shared_ptr<Application> CreateApp();
}

class AppWindow : public BaseWindow
{
public:
	AppWindow( HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL );

protected:
	virtual LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif