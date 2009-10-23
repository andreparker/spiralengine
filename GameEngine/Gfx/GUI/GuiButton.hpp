#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "GuiWindow.hpp"

namespace Spiral
{

namespace GUI
{
	class GuiButton : public GuiWindow
	{
	public:
		GuiButton( const Math::SpVector2r& position, const Rect< SpReal >& rect, 
			const Rect< SpReal >& textCoords,const boost::shared_ptr< Texture >& texture, bool bAlpha );
		virtual ~GuiButton();

		bool IsButtonDown()const
		{
			return m_buttonDown;
		}

	private:
		bool m_buttonDown;

		void OnMouseDown( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnMouseUp( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnMouseHover( boost::int32_t eventId, GuiWindow* window, const boost::any& data );

		virtual void ResetWindow();
	};
}
}
#endif