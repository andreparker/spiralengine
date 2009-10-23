#ifndef PLATFORM_WINDOWS_HPP
#define PLATFORM_WINDOWS_HPP

#include "../../Core/Application.hpp"
#include "WindowsLogger.hpp"
#include "BaseWindow.h"
#include "../../Core/EventPublisher.hpp"
#include "../../Core/EventSubscriber.hpp"
#include "../../Core/Event.hpp"
#include "../../Core/Events.hpp"

namespace Spiral
{
	boost::shared_ptr<Application> CreateApp();
}

class AppWindow : public BaseWindow
{
public:
	AppWindow( HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL );

	void Initialize();
	void KeyUpCallback( WPARAM wParam, LPARAM lParam );
	void KeyDownCallback( WPARAM wParam, LPARAM lParam );
	void LeftMouseDownCallBack( WPARAM wParam, LPARAM lParam );
	void LeftMouseUpCallBack( WPARAM wParam, LPARAM lParam );
	void MouseMoveCallBack( WPARAM wParam, LPARAM lParam );

	void SetEventPublisher( const boost::shared_ptr< Spiral::EventPublisher >& publisher )
	{
		m_eventPublisher = publisher;
	}

	void SysAppEvents( const Spiral::Event& event, const boost::any& data );
	
	bool Quit()const
	{
		return m_quit;
	}
protected:
	virtual LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	boost::shared_ptr< Spiral::EventPublisher > m_eventPublisher;
	boost::shared_ptr< Spiral::EventSubscriber > m_sysAppEventSubscriber;
	bool m_quit;
};

#endif