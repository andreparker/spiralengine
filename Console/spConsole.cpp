///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "spConsole.h"
#include <windows.h>
#include <boost/tokenizer.hpp>
#include <boost/bind.hpp>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////
using namespace Spiral::Console;

typedef boost::tokenizer< boost::char_separator<char> >::iterator iterator;

SpConsole::SpConsole( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetForegroundColour( wxColour( 3, 26, 129 ) );
	this->SetBackgroundColour( wxColour( 3, 26, 129 ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_richText1 = new wxRichTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxALWAYS_SHOW_SB|wxNO_BORDER|wxVSCROLL );
	m_richText1->SetForegroundColour( wxColour( 3, 26, 129 ) );
	m_richText1->SetBackgroundColour( wxColour( 3, 26, 129 ) );
	m_richText1->SetFont( wxFont( 11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
	m_richText1->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( SpConsole::OnClick ), NULL, this );
	m_richText1->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( SpConsole::OnMouseDown ), NULL, this );
	m_richText1->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( SpConsole::OnMouseUp ), NULL, this );
	m_caretPos = 0;

	bSizer1->Add( m_richText1, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();

	this->Show();
}

SpConsole::~SpConsole()
{
	m_richText1->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( SpConsole::OnClick ), NULL, this );
	m_richText1->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( SpConsole::OnMouseDown ), NULL, this );
	m_richText1->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( SpConsole::OnMouseUp ), NULL, this );
}

WXLRESULT SpConsole::MSWWindowProc( WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam )
{
	if( nMsg == WM_COPYDATA )
	{
		COPYDATASTRUCT* data = reinterpret_cast< COPYDATASTRUCT* >( lParam );
		if( data )
		{
			const wxChar* str = reinterpret_cast< const wxChar* >( data->lpData );
			size_t len = data->cbData;
			wxString wxstr( str, len );

			if( wxstr.Freq('^') )
			{
				ProcessText( wxstr );
			}else
			{
				m_richText1->BeginTextColour( *wxWHITE );
				m_richText1->WriteText( wxstr );
				m_richText1->EndTextColour();
			}
		
			m_caretPos = m_richText1->GetCaretPosition();
		}

		return 0;
	}else
	{
		return wxFrame::MSWWindowProc( nMsg, wParam, lParam );
	}
}


void DrawStringColour( wxRichTextCtrl* ctrl, const wxColour& clr, const std::string& str )
{
	ctrl->BeginTextColour( clr );
	ctrl->WriteText( wxString( str.c_str(), str.length() ) );
	ctrl->EndTextColour();
}

void DrawString( wxRichTextCtrl* ctrl, const std::string& str )
{
	char c = *(str.c_str());
	std::string _str( (str.c_str() + 1), str.length()-1 );

	switch( c )
	{
	case 'r':
		DrawStringColour( ctrl, *wxRED, _str );
		break;
	case 'b':
		DrawStringColour( ctrl, *wxBLUE, _str );
		break;
	case 'g':
		DrawStringColour( ctrl, *wxGREEN, _str );
		break;
	case 'w':
		DrawStringColour( ctrl, *wxWHITE, _str );
		break;
	case 'd':
		DrawStringColour( ctrl, *wxBLACK, _str );
		break;
	case 'l':
		DrawStringColour( ctrl, *wxLIGHT_GREY, _str );
		break;
	case 'y':
		DrawStringColour( ctrl, wxColour( 255,255,0 ), _str );
		break;
	default:
		DrawStringColour( ctrl, *wxWHITE, _str );
		break;
	}
}

void SpConsole::ProcessText( const wxString& str )
{
	const boost::char_separator<char> delims( "^" );
	std::string cstr( str.c_str(), str.length() );
	boost::tokenizer< boost::char_separator<char> > tokens( cstr, delims );

	std::for_each( tokens.begin(), tokens.end(), boost::bind( DrawString, m_richText1, _1 ) );
}

void SpConsole::OnMouseDown( wxMouseEvent& event )
{
	event.Skip(true);
}

void SpConsole::OnMouseUp( wxMouseEvent& event )
{
	m_richText1->SetCaretPosition( m_caretPos, true );
	event.Skip(true);
}

void SpConsole::OnClick( wxMouseEvent& event )
{
	m_richText1->SetCaretPosition( m_caretPos, true );
	event.Skip(true);
}