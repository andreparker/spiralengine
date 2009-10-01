#ifndef GUI_HPP
#define GUI_HPP

#include "../../Core/EventSubscriber.hpp"
#include <boost/shared_ptr.hpp>
#include <list>

namespace Spiral
{

	class GfxDriver;
namespace GUI
{
	class GuiWindow;

	class GuiManager
	{
	public:
		GuiManager();

		void AddWindow( const boost::shared_ptr< GuiWindow >& window );
		void RemoveWindow( const boost::shared_ptr< GuiWindow >& window );
		void Present( const boost::shared_ptr< GfxDriver >& gfxDrvier );

		void MouseInput( const Event& mouseEvent, const boost::any& data );
	private:
		void TraverseRender( const boost::shared_ptr< GfxDriver >& gfxDrvier,const boost::shared_ptr< GuiWindow >& window )const;

		typedef std::list< boost::shared_ptr< GuiWindow > >::iterator gui_window_itr;
		typedef std::list< boost::shared_ptr< GuiWindow > >::const_iterator const_gui_window_itr;

		std::list< boost::shared_ptr< GuiWindow > > m_windowList;
	};
}

}
#endif