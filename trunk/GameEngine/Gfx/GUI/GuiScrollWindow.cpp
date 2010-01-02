#include "GuiScrollWindow.hpp"
#include "GuiSlider.hpp"
#include "Gui.hpp"
#include "GuiWindowEvents.hpp"
#include "../Texture.hpp"

#include <boost/bind.hpp>

using namespace Spiral;
using namespace Spiral::GUI;

GuiScrollWindow::GuiScrollWindow( const Math::SpVector2r& position, const Rect<SpReal>& rect,boost::uint32_t scrollBarThickness, 
								 const boost::shared_ptr<GuiWindow>& scrollContents, const boost::shared_ptr<GuiManager>& guiManager ):
GuiWindow( position, rect, Rect<SpReal>(), boost::shared_ptr<Texture>(), false ),m_scrollContents(),m_scrollBarThickness(scrollBarThickness)
{
	const SpReal thickness = static_cast<SpReal>(scrollBarThickness);
	m_scrollArea = boost::shared_ptr<GuiWindow>( new GuiWindow( Math::make_vector(0.0f,0.0f), 
		Rect<SpReal>(0.0f,rect.right,rect.bottom, 0.0f ),Rect<SpReal>(),boost::shared_ptr<Texture>(),false ));

	m_scrollVBar = guiManager->Make_DefSlider( rect.right - thickness, 0.0f, thickness, rect.bottom - thickness, 16, GuiSliderDir::VerticalDir() );
	m_scrollHBar = guiManager->Make_DefSlider( 0.0f, rect.bottom - thickness, rect.right - thickness, thickness, 16, GuiSliderDir::HorizontalDir() );

	m_scrollHBar->SetRange( 0, 100 );
	m_scrollVBar->SetRange( 0, 100 );
	m_scrollHBar->ConnectHandler( data_changed, boost::bind( &GuiScrollWindow::OnScrollH, this, _1, _2, _3 ) );
	m_scrollVBar->ConnectHandler( data_changed, boost::bind( &GuiScrollWindow::OnScrollV, this, _1, _2, _3 ) );

	AddChild( m_scrollVBar );
	AddChild( m_scrollHBar );
	AddChild( m_scrollArea );

	m_scrollArea->Show();
	m_scrollArea->SetClipChildren();

	if( scrollContents )
	{
		AttachWindow( scrollContents );
	}
}

void GuiScrollWindow::AttachWindow( const boost::shared_ptr<GuiWindow>& window )
{
	if( window && !m_scrollContents )
	{
		Rect<SpReal> rect = window->GetRect();
		Rect<SpReal> bkRect = GetRect();
		Rect<SpReal> scrollAreaRct = bkRect;

		SpReal width = rect.right - bkRect.right;
		SpReal height = rect.bottom - bkRect.bottom;

		if( width > 0.0f )
		{
			scrollAreaRct.bottom -= m_scrollBarThickness;

			SpReal right = (bkRect.right-m_scrollBarThickness);
			SpReal pc = right / rect.right;
			Rect<SpReal> rct = m_scrollHBar->GetRect();
			rct.right = pc * right;
			rct.bottom = static_cast<SpReal>(m_scrollBarThickness);

			m_scrollHBar->SetSliderRect( rct );
			m_scrollHBar->Show();
		}

		if( height > 0.0f )
		{
			scrollAreaRct.right -= m_scrollBarThickness;

			SpReal bottom = (bkRect.bottom-m_scrollBarThickness);
			SpReal pc = bottom / rect.bottom;
			Rect<SpReal> rct = m_scrollVBar->GetRect();
			rct.bottom = bottom * pc;
			rct.right = static_cast<SpReal>(m_scrollBarThickness);

			m_scrollVBar->SetSliderRect( rct );
			m_scrollVBar->Show();
		}

		m_scrollArea->SetRect( scrollAreaRct ); // change the scroll area rect to accomidate the scroll bars
		m_scrollContents = window;
		m_scrollContents->SetLocalPosition( Math::make_vector( 0.0f, 0.0f ) );
		m_scrollArea->AddChild( window );
	}
}

void GuiScrollWindow::DetachWindow()
{
	if( m_scrollContents )
	{
		m_scrollArea->RemoveChild( m_scrollContents );
		m_scrollContents.reset();
	}
}

void GuiScrollWindow::OnScrollH( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	SpReal pc = static_cast<SpReal>(m_scrollHBar->GetSliderPos())/m_scrollHBar->GetMax();
	Rect<SpReal> rct = m_scrollContents->GetRect();
	Rect<SpReal> area_rct = m_scrollArea->GetRect();

	SpReal width = (rct.right - area_rct.right ) * pc;
	Math::SpVector2r pos = m_scrollContents->GetLocalPosition();

	pos[0] = -width;
	m_scrollContents->SetLocalPosition( pos );
}

void GuiScrollWindow::OnScrollV( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	SpReal pc = static_cast<SpReal>(m_scrollVBar->GetSliderPos())/m_scrollVBar->GetMax();
	Rect<SpReal> rct = m_scrollContents->GetRect();
	Rect<SpReal> area_rct = m_scrollArea->GetRect();

	SpReal height = (rct.bottom - area_rct.bottom) * pc;
	Math::SpVector2r pos = m_scrollContents->GetLocalPosition();

	pos[1] = -height;
	m_scrollContents->SetLocalPosition( pos );
}