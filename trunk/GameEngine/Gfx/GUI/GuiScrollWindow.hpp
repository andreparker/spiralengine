#ifndef GUI_SCROLL_WINDOW_HPP
#define GUI_SCROLL_WINDOW_HPP

#include "GuiWindow.hpp"

namespace Spiral
{

namespace GUI
{
	class GuiSlider;
	class GuiManager;
	class GuiScrollWindow : public GuiWindow
	{
	public:
		GuiScrollWindow( const Math::Vector2f& position, const Rect<SpReal>& rect,boost::uint32_t scrollBarThickness, 
			             const boost::shared_ptr<GuiWindow>& scrollContents, const boost::shared_ptr<GuiManager>& guiManager );

		void AttachWindow( const boost::shared_ptr<GuiWindow>& window );
		void DetachWindow();
	private:

		void OnScrollH( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnScrollV( boost::int32_t eventId, GuiWindow* window, const boost::any& data );

	private:
		boost::shared_ptr<GuiSlider> m_scrollVBar;
		boost::shared_ptr<GuiSlider> m_scrollHBar;
		boost::shared_ptr<GuiWindow> m_scrollArea;
		boost::shared_ptr<GuiWindow> m_scrollContents;
		boost::uint32_t              m_scrollBarThickness;

	};
}
}
#endif