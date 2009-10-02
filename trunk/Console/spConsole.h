///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __spConsole__
#define __spConsole__

#include <wx/richtext/richtextctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace Spiral
{
	namespace Console
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class SpConsole
		///////////////////////////////////////////////////////////////////////////////
		class SpConsole : public wxFrame 
		{
			private:
				wxRichTextCtrl* m_richText1;
			
				void ProcessText( const wxString& str );
			protected:

				virtual void OnClick( wxMouseEvent& event );
				virtual void OnMouseDown( wxMouseEvent& event );
				virtual void OnMouseUp( wxMouseEvent& event );

			public:
				SpConsole( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Console"), const wxPoint& pos = wxDefaultPosition, 
							const wxSize& size = wxSize( 699,638 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_NO_TASKBAR|wxSYSTEM_MENU|wxNO_BORDER,
							const wxString& name = wxT("Console") );
				~SpConsole();
			
				WXLRESULT MSWWindowProc(WXUINT nMsg,
					WXWPARAM wParam, WXLPARAM lParam);
		private:
			long m_caretPos;

		};
		
	} // namespace Console
} // namespace Spiral

#endif //__spConsole__
