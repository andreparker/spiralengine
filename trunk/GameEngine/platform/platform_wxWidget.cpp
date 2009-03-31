#include <vld.h>
#include "platform_wxWidget.hpp"
#include "platform_wxConsole.hpp"
#include "../core/logger.hpp"

#include <wx/display.h>

IMPLEMENT_APP( platform_app );

using namespace gameEngine::common;
using namespace gameEngine;

namespace
{
	bool init_logsystem()
	{
		LogRouter::instance().addLogger( &ConsoleLog::instance() );

		return true;
	}
}

app_frame::app_frame(const wxString &name):
m_name(name)
{
}

bool app_frame::createTextCtrl()
{
	m_textFrame = new wxFrame
		( NULL, 
			-1, 
			"Engine Console",
			wxDefaultPosition,
			wxSize( 400, 600 ) );

	if( m_textFrame )
	{
		m_textFrame->Show();
		m_textFrame->SetBackgroundColour( wxColour( 255, 255, 255, 255 ) );
		m_textFrame->Refresh();

		m_text = new wxRichTextCtrl
		( m_textFrame,
			-1,
			"",
			wxDefaultPosition,
			wxSize( 400, 600 ),32 | wxTRANSPARENT_WINDOW );

		

		if( m_text )
		{
			m_text->SetForegroundColour( wxColor(255,255,255,255) );
			m_text->SetBackgroundColour( m_textFrame->GetBackgroundColour() );
			m_text->SetBackgroundStyle( wxBG_STYLE_COLOUR );
			m_text->SetEditable( false );
			m_text->Show();
			m_text->Refresh();
		}
	}
	
	
	
	return m_text ? true : false;
}

void app_frame::OnClose( wxCloseEvent& e )
{
	Destroy();
}

platform_app::platform_app():m_frame( NULL ),m_gameApp(NULL),m_isFullScreen(false)
{
}

wxRichTextCtrl* platform_app::getConsole()const
{
	return m_frame->getTextControl();
}

bool platform_app::OnInit()
{
	bool windowStatus;

	// initialize RTTI and objects
	objectManager* objectMgr = objectManager::instance();
	objectMgr->initialize();

	// get the game application
	m_gameApp = frameWork::getApplication();
	m_frame = new app_frame( m_gameApp->getAppName() );

	gameEngine::Size2 size = m_gameApp->getResolution();
	// create our main window frame
	windowStatus = m_frame->Create
	( NULL,
	  -1,
	  m_frame->getName(),
	  wxPoint( -1,-1 ),
	  wxSize( size.x, size.y ),
	  ( wxDEFAULT_FRAME_STYLE & ~ (wxRESIZE_BORDER | wxRESIZE_BOX | wxMAXIMIZE_BOX) ),
	  m_frame->getName() );


	if( windowStatus )
	{
		// set the window to be shown
		m_frame->Show();
		SetTopWindow( m_frame );

		windowStatus = m_frame->createTextCtrl();

		if( windowStatus )
		{
			windowStatus = init_logsystem();
			LOG_I( " - Main Window initialized\n" );
			LOG_I( " - width: %d, height: %d\n", size.x, size.y );
		}
	}
	
	
	// start a timer
	m_watch.Start();
	objectMgr->unReference();
	return m_gameApp->initialize() && windowStatus;
}

int platform_app::OnExit()
{
	if( m_gameApp )
	{
		m_gameApp->unInitialize();
		m_gameApp->destroy();
		m_gameApp = NULL;

		objectManager* objectMgr = objectManager::instance();
		objectMgr->destroy();
	}

	m_frame = NULL;

	return 0;
}

void platform_app::changeDisplay( int width, int height, bool fullscreen )
{
	wxDisplay display;
	if( fullscreen )
	{
		wxVideoMode mode = display.GetCurrentMode();

		mode.w = width;
		mode.h = height;

		display.ChangeMode( mode );
	}else
		display.ChangeMode();

	m_frame->ShowFullScreen( fullscreen );
}

void platform_app::update( wxIdleEvent& e )
{
	long currentDt = m_watch.Time();
	m_watch.Start();

	float seconds = static_cast< float >( currentDt ) / 1000.0f;

	if( m_gameApp ) m_gameApp->updateApp( seconds );

	// try to get as much idle time as possible
	e.RequestMore();

}

void platform_app::keyDown( wxKeyEvent& e )
{
	if( e.HasModifiers() )
	{
		if( e.GetKeyCode() == WXK_RETURN &&
			e.GetModifiers() & wxMOD_ALT )
		{
			// change display
			m_isFullScreen = !m_isFullScreen;
			gameEngine::Size2 size = m_gameApp->getResolution();
			changeDisplay( size.x, size.y, m_isFullScreen );

			// tell game app that screen has changed
			// TODO: update the app from screen change
		}
	}
}

BEGIN_EVENT_TABLE( app_frame, wxFrame )
	EVT_CLOSE( app_frame::OnClose )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( platform_app, wxApp )
	EVT_IDLE( platform_app::update )
	EVT_KEY_DOWN( platform_app::keyDown )
END_EVENT_TABLE()

