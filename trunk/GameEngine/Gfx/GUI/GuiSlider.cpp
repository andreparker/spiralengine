#include "GuiSlider.hpp"
#include "../../Math/Math.hpp"
#include "GuiWindowEvents.hpp"

#include <boost/any.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

using namespace Spiral;
using namespace Spiral::GUI;

GuiSlider::GuiSlider( const Math::SpVector2r& position, const Rect<SpReal>& rect, const Rect<SpReal>& bgTexCoords,
					 const Rect<SpReal>& sliderTexCoords, boost::uint32_t sliderSize, const boost::shared_ptr<Texture>& bgTexture, 
					 const boost::shared_ptr<Texture>& sliderTexture, const GuiSliderDir& dir, bool bBgAlpha, bool bSliderAlpha ):
GuiWindow( position, rect, bgTexCoords, bgTexture, bBgAlpha ),m_slider(),m_sliderSize(sliderSize),m_sliderDir(dir),m_minRange(0),
m_maxRange(10),m_mouseDown(false)
{
	boost::uint32_t width = sliderSize,height = sliderSize;

	if( m_sliderDir.GetDir() == GuiSliderDir::Horizontal )
	{
		width /= 2;
	}else
	{
		height /= 2;
	}

	Rect<SpReal> sliderRect( 0.0f, static_cast<SpReal>(width), static_cast<SpReal>(height), 0.0f );
	m_slider = boost::make_shared< GuiWindow >( Math::make_vector( 0.0f, 0.0f ), sliderRect, sliderTexCoords, sliderTexture, bSliderAlpha );
	m_slider->Show();

	AddChild( m_slider );

	m_slider->ConnectHandler( mouse_down, boost::bind( &GuiSlider::OnMouseDownSlider, this, _1, _2, _3 ) );
	m_slider->ConnectHandler( mouse_up, boost::bind( &GuiSlider::OnMouseUpSlider, this, _1, _2, _3 ) );
	m_slider->ConnectHandler( mouse_move, boost::bind( &GuiSlider::OnMouseMove, this, _1, _2, _3 ) );
	m_slider->ConnectHandler( focus_lost, boost::bind( &GuiSlider::OnFocusLost, this, _1, _2, _3 ) );

	ConnectHandler( mouse_down, boost::bind( &GuiSlider::OnMouseDown, this, _1, _2, _3 ) );
	//ConnectHandler( mouse_hover, boost::bind( &GuiSlider::OnMouseHover, this, _1, _2, _3 ) );

}

GuiSlider::~GuiSlider()
{
	m_slider->DisConnectHandler( mouse_down );
	m_slider->DisConnectHandler( mouse_up );
}

void GuiSlider::SetRange( boost::int32_t min_, boost::int32_t max_ )
{
	m_minRange = min_;
	m_maxRange = max_;
}

boost::int32_t GuiSlider::GetSliderPos() const
{
	Math::SpVector2r position = m_slider->GetLocalPosition(), delta( 0.0f, 0.0f );
	Rect<SpReal> bounds = GetRect();
	Rect<SpReal> slider_bounds = m_slider->GetRect();
	SpReal percent,dist;

	if( m_sliderDir.GetDir() == GuiSliderDir::Horizontal )
	{
		dist = (bounds.right - bounds.left) - slider_bounds.right;
		percent = position[0] / dist;
	}else
	{
		dist = (bounds.bottom - bounds.top) - slider_bounds.bottom;
		percent = position[1] / dist;
	}

	return static_cast<boost::int32_t>( ( m_maxRange - m_minRange ) * percent ) + m_minRange;
}

void GuiSlider::OnMouseDown( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	const mouse_position pos = boost::any_cast< const mouse_position >( data );
	Math::SpVector2r position = WorldToLocal( pos.x, pos.y );
	Math::SpVector2r sliderPos = m_slider->GetLocalPosition();
	if( m_sliderDir.GetDir() == GuiSliderDir::Horizontal )
	{
		sliderPos[0] = position[0];
	}else
	{
		sliderPos[1] = position[1];
	}

	m_slider->SetLocalPosition( sliderPos );
	SliderBoundsCheck();

	// report change
	CallHandler( data_changed, this, boost::any() );
}

void GuiSlider::OnMouseHover( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	if( m_mouseDown )
	{
		OnMouseDown( eventId, window, data );
	}
}

void GuiSlider::OnMouseDownSlider( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	m_mouseDown = true;
	const mouse_position pos = boost::any_cast< const mouse_position >( data );
	SaveLastPosition( pos );

}

void GuiSlider::OnMouseUpSlider( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	m_mouseDown = false;
}


void GuiSlider::SaveLastPosition( const mouse_position& pos_ )
{
	if( m_sliderDir.GetDir() == GuiSliderDir::Horizontal )
	{
		m_lastPos = pos_.x;
	}else
	{
		m_lastPos = pos_.y;
	}

}

Spiral::SpReal GuiSlider::GetDelta( const mouse_position& pos_ ) const
{
	SpReal delta;
	if( m_sliderDir.GetDir() == GuiSliderDir::Horizontal )
	{
		delta = pos_.x - m_lastPos;
	}else
	{
		delta = pos_.y - m_lastPos;
	}

	return delta;
}

void GuiSlider::MoveSlider( SpReal delta )
{
	Math::SpVector2r position = m_slider->GetLocalPosition();
	if( m_sliderDir.GetDir() == GuiSliderDir::Horizontal )
	{
		position[0] += delta;
	}else
	{
		position[1] += delta;
	}

	m_slider->SetLocalPosition( position );
}

void GuiSlider::SliderBoundsCheck()
{
	Math::SpVector2r position = m_slider->GetLocalPosition(), delta( 0.0f, 0.0f );
	Rect<SpReal> bounds = GetRect();
	Rect<SpReal> slider_bounds = m_slider->GetRect();
	SpReal minDelta,maxDelta;

	if( m_sliderDir.GetDir() == GuiSliderDir::Horizontal )
	{
		minDelta = position[0] - bounds.left;
		maxDelta = (position[0] + slider_bounds.right)- bounds.right;

		if( minDelta < 0 )
		{
			delta = Math::make_vector( minDelta, 0.0f );
		}else if( maxDelta > 0 )
		{
			delta = Math::make_vector( maxDelta, 0.0f );
		}
	}else
	{
		minDelta = position[1] - bounds.top;
		maxDelta = (position[1] + slider_bounds.bottom) - bounds.bottom;

		if( minDelta < 0 )
		{
			delta = Math::make_vector( 0.0f, minDelta );
		}else if( maxDelta > 0 )
		{
			delta = Math::make_vector( 0.0f, maxDelta );
		}
	}
	position -= delta;
	m_slider->SetLocalPosition( position );
}


void GuiSlider::ResetWindow()
{
	//m_mouseDown = false;
}



void GuiSlider::OnMouseMove( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	const mouse_position pos = boost::any_cast< const mouse_position >( data );

	if( m_mouseDown )
	{
		SpReal delta = GetDelta( pos );
		SaveLastPosition( pos );
		MoveSlider( delta );
		SliderBoundsCheck();

		if( static_cast<boost::int32_t>(delta) != 0 )
		{
			// report change
			CallHandler( data_changed, this, boost::any() );
		}
		
	}
}

void GuiSlider::OnFocusLost( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	m_mouseDown = false;
}

void GuiSlider::SetSliderRect( const Rect<SpReal>& rect )
{
	m_slider->SetRect( rect );
}