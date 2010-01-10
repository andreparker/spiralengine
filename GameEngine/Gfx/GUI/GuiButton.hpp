#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "GuiWindow.hpp"

namespace Spiral
{

namespace GUI
{
	class GuiManager;
	class GuiButton : public GuiWindow
	{
	public:
		GuiButton( const Math::Vector2f& position, const Rect< SpReal >& rect, 
			const Rect< SpReal >& textCoords,const boost::shared_ptr< Texture >& texture, bool bAlpha );
		GuiButton();
		virtual ~GuiButton();

		bool IsButtonDown()const
		{
			return m_buttonDown;
		}

		void SetButtonPressScript( const std::string& script )
		{
			m_buttonPressScript = script;
		}

		void SetGuiManager( GuiManager* gui )
		{
			m_gui = gui;
		}
	private:
		bool m_buttonDown;
		bool m_hoverCoordsSet;

		void SetupHandlers();
		void OnButtonPressScript();
		void OnMouseDown( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnMouseUp( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnMouseHover( boost::int32_t eventId, GuiWindow* window, const boost::any& data );

		virtual void ResetWindow();
	private:

		std::string m_buttonPressScript;
		GuiManager* m_gui;
	};
}
}
#endif