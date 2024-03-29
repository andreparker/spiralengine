#ifndef GUI_FRAME_HPP
#define GUI_FRAME_HPP

#include "GuiWindow.hpp"

namespace Spiral
{

namespace GUI
{

	struct mouse_position;
	class GuiFrame : public GuiWindow
	{
	public:
		GuiFrame( const Math::Vector2f& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords,
			      const boost::shared_ptr< Texture >& texture, bool bAlpha );
		GuiFrame();

		virtual void SetRect( const Rect< SpReal >& rt );
	private:

		void OnMouseDownBar( boost::int32_t eventId, GuiWindow* window, const boost::any& data  );
		void OnMouseUpBar( boost::int32_t eventId, GuiWindow* window, const boost::any& data  );
		void OnMouseMove(  boost::int32_t eventId, GuiWindow* window, const boost::any& data );

		void SaveLastPosition( const mouse_position& pos );
		virtual void ResetWindow();

		void CreateFrameBar( const Rect< SpReal > &rect, const Rect< SpReal >& textCoords, bool bAlpha );
	private:
		boost::shared_ptr<GuiWindow> m_frameBar;
		SpReal m_posX,m_posY;
		bool m_mouseDown;
	};
}
}
#endif