#ifndef GUI_SLIDER_HPP
#define GUI_SLIDER_HPP

#include "GuiWindow.hpp"

namespace Spiral
{

namespace GUI
{
	struct mouse_position;
	struct GuiSliderDir
	{
		GuiSliderDir():dir(INVALID){}

		typedef enum
		{
			INVALID = -1,
			Vertical = 1,
			Horizontal
		}SliderDirType;

		SliderDirType GetDir()const
		{
			return dir;
		}

		static GuiSliderDir VerticalDir()
		{
			return GuiSliderDir( Vertical );
		}

		static GuiSliderDir HorizontalDir()
		{
			return GuiSliderDir( Horizontal );
		}
	private:
		SliderDirType dir;
		GuiSliderDir( SliderDirType type ):dir(type){}
	};

	class GuiSlider : public GuiWindow
	{
	public:
		GuiSlider( const Math::SpVector2r& position, const Rect<SpReal>& rect, const Rect<SpReal>& bgTexCoords,
			const Rect<SpReal>& sliderTexCoords, boost::uint32_t sliderSize, const boost::shared_ptr<Texture>& bgTexture,
			const boost::shared_ptr<Texture>& sliderTexture, const GuiSliderDir& dir, bool bBgAlpha, bool bSliderAlpha );

		~GuiSlider();
		void SetRange( boost::int32_t min_, boost::int32_t max_ );
		boost::int32_t GetSliderPos()const;
	private:

		void OnMouseDown( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnMouseHover( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnMouseDownSlider( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnMouseUpSlider( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnMouseHoverSlider( boost::int32_t eventId, GuiWindow* window, const boost::any& data );

		void SaveLastPosition( const mouse_position& pos_ );
		SpReal GetDelta( const mouse_position& pos_ )const;
		void MoveSlider( SpReal delta );
		void SliderBoundsCheck();

		virtual void ResetWindow();
	private:
		boost::shared_ptr<GuiWindow> m_slider;
		const boost::uint32_t m_sliderSize;
		const GuiSliderDir m_sliderDir;
		boost::int32_t m_minRange, m_maxRange;
		bool m_mouseDown;
		SpReal m_lastPos;
	};
}
}
#endif