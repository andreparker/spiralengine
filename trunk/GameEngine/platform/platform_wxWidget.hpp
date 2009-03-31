#ifndef PLATFORM_WXWIDGET_HPP
#define PLATFORM_WXWIDGET_HPP

#include "../core/types.hpp"
#include "../core/objectmanager.hpp"
#include "../core/app.hpp"

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/stopwatch.h>
#include <wx/richtext/richtextctrl.h>

class app_frame : public wxFrame
{
	public:
		app_frame( const wxString& name );

		const wxString& getName()const { return m_name; }
		bool createTextCtrl();
		wxRichTextCtrl* getTextControl()const { return m_text; }
	protected:
		wxString m_name;
		wxRichTextCtrl* m_text;
		wxFrame* m_textFrame;


		void OnClose( wxCloseEvent& e );
	DECLARE_EVENT_TABLE()
};

class platform_app : public wxApp
{
	public:
		platform_app();
		bool OnInit();
		int OnExit();

		bool isFullScreen()const { return m_isFullScreen; }
		void changeDisplay( int width, int height, bool fullscreen = true );

		void update(  wxIdleEvent& e );
		void keyDown( wxKeyEvent& e );
		wxRichTextCtrl* getConsole()const;
	protected:
		app_frame* m_frame;
		gameEngine::frameWork::coreApp* m_gameApp;
		wxStopWatch m_watch;
		bool m_isFullScreen;
	DECLARE_EVENT_TABLE()
};

DECLARE_APP( platform_app )

#endif