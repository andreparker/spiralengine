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
	struct mouse_position;

	class GuiManager
	{
	public:
		GuiManager();

		/*!
		   @function  AddElement
		   @brief     adds a window
		   @return    void
		   @param     const boost::shared_ptr< GuiWindow > & window
		*/
		void AddElement( const boost::shared_ptr< GuiWindow >& window );

		/*!
		   @function  RemoveElement
		   @brief     removes a window
		   @return    void
		   @param     const boost::shared_ptr< GuiWindow > & window
		*/
		void RemoveElement( const boost::shared_ptr< GuiWindow >& window );

		/*!
		   @function  Present
		   @brief     draws windows in the gui
		   @return    void
		   @param     const boost::shared_ptr< GfxDriver > & gfxDrvier
		*/
		void Present( const boost::shared_ptr< GfxDriver >& gfxDrvier );

		/*!
		   @function  Input
		   @brief     sends input to windows
		   @return    void
		   @param     const Event & inputEvent
		   @param     const boost::any & data
		*/
		void Input( const Event& inputEvent, const boost::any& data );

		/*!
		   @function  Clear
		   @brief     clears all elements from the gui manager
		   @return    void
		*/
		void Clear();

	private:
		
		/*!
		   @function  HandleMouseInput
		   @brief     calls mouse handlers
		   @return    void
		   @param     const Event & inputEvent
		   @param     const mouse_position & position
		*/
		void HandleMouseInput( const Event& inputEvent, const mouse_position& position );

		/*!
		   @function  HandleCharInput
		   @brief     character input
		   @return    void
		   @param     const Event & inputEvent
		   @param     boost::uint32_t char_
		*/
		void HandleCharInput( const Event& inputEvent, boost::uint32_t char_ );

		void TraverseRender( const boost::shared_ptr< GfxDriver >& gfxDrvier,const boost::shared_ptr< GuiWindow >& window )const;

		typedef std::list< boost::shared_ptr< GuiWindow > >::iterator gui_window_itr;
		typedef std::list< boost::shared_ptr< GuiWindow > >::const_iterator const_gui_window_itr;

		std::list< boost::shared_ptr< GuiWindow > > m_windowList;
	};
}

}
#endif