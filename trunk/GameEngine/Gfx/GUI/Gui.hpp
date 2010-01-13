#ifndef GUI_HPP
#define GUI_HPP

#include "../../Core/Sp_DataTypes.hpp"
#include "../../Core/EventSubscriber.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/function.hpp>
#include <map>
#include <list>

namespace Spiral
{

	class GfxDriver;
	class Font;
	class Rgba;

	class IFile;
	class Engine;
	class ScriptManager;

namespace GUI
{
	class GuiWindow;
	class GuiButton;
	class GuiCheckBox;
	class GuiText;
	class GuiSlider;
	class GuiEditBox;
	class GuiFrame;

	struct mouse_position;
	struct GuiSliderDir;

	class GuiManager
	{
	public:
		GuiManager( Engine* engine );

		/*!
		   @function  FindWindow
		   @brief     finds a window in the list
		   @return    GuiWindow*
		   @param     const std::string & name
		*/
		GuiWindow* FindWindow( const std::string& name );

		/*!
		   @function  Register
		   @brief     allow script to access the gui
		   @return    void
		   @param     const boost::shared_ptr<ScriptManager> & scriptMgr
		*/
		void Register( const boost::shared_ptr<ScriptManager>& scriptMgr );

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

		void Delay_Clear();
		/*!
		   @function  LoadLayout
		   @brief     loads a .layout file containing gui elements to create
		   @return    bool
		   @param     const boost::shared_ptr<IFile> & layout
		*/
		bool LoadLayout( const boost::shared_ptr<IFile>& layout );

		void LoadLayout_Script( const std::string& fileName );

		void Delay_LoadLayout_Script( const std::string& fileName );

		Engine* GetEngine()const
		{
			return m_pImplEngine;
		}

		boost::shared_ptr< GuiButton >   Make_DefButton( SpReal posX, SpReal posY, SpReal width, SpReal height );
		boost::shared_ptr< GuiWindow >   Make_DefWindow( SpReal posX, SpReal posY, SpReal width, SpReal height );
		boost::shared_ptr< GuiFrame >    Make_DefFrame( SpReal posX, SpReal posY, SpReal width, SpReal height );
		boost::shared_ptr< GuiCheckBox > Make_DefCheckBox( SpReal posX, SpReal posY, SpReal width, SpReal height );
		boost::shared_ptr< GuiText >     Make_DefTextBox( SpReal posX, SpReal posY, const Rgba& fontColor, 
			             const boost::shared_ptr<Font>& font, boost::uint32_t maxAllowedChar, const wString& text );
		boost::shared_ptr< GuiSlider >   Make_DefSlider( SpReal posX, SpReal posY, SpReal width, SpReal height, 
			                                             boost::uint32_t sliderSize, const GuiSliderDir& dir );
		boost::shared_ptr< GuiEditBox >  Make_DefEditBox( SpReal posX, SpReal posY, const Rgba& bkColor, const Rgba& fontColor, 
			                             const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const wString& text );

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

		void PlaceFocusWindow();
	private:

		bool IsElement( const std::string& type )const
		{
			return bool( type == "eClass" );
		}

		bool LoadSubLayout( const boost::shared_ptr< GuiWindow >& window, const boost::shared_ptr< IFile >& layoutFile )const;
		bool LoadLayoutImpl( const boost::shared_ptr< IFile >& layoutFile, const boost::function< void( const boost::shared_ptr<GuiWindow>& ) >& storeFunction )const;

		boost::shared_ptr< GuiWindow > Create_Element( const std::string& type, const std::string& parentPath, const boost::property_tree::ptree& tree )const;
		boost::shared_ptr< GuiWindow > Create_Window_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree )const;
		boost::shared_ptr< GuiWindow > Create_Button_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree )const;
		boost::shared_ptr< GuiWindow > Create_Frame_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree )const;
		boost::shared_ptr< GuiWindow > Create_EditBox_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree )const;
		boost::shared_ptr< GuiWindow > Create_TextBox_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree )const;
		boost::shared_ptr< GuiWindow > Create_Scroll_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree )const;
		boost::shared_ptr< GuiWindow > Create_Slider_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree )const;
		boost::shared_ptr< GuiWindow > Create_CheckBox_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree )const;

		void BaseWindowAttributes( boost::property_tree::ptree::const_iterator& itr, const boost::shared_ptr< GuiWindow >& newWindow, const std::string& parentPath )const;
		void OverRideAttributes( const std::string& parentPath, boost::property_tree::ptree::const_iterator& itr )const;


	private:
		typedef std::list< boost::shared_ptr< GuiWindow > >::iterator gui_window_itr;
		typedef std::list< boost::shared_ptr< GuiWindow > >::const_iterator const_gui_window_itr;
		typedef	boost::function< boost::shared_ptr< GuiWindow >( const std::string&, const boost::property_tree::ptree& ) > create_call;

		typedef std::map< std::string, create_call >::iterator ElemItr;
		typedef std::map< std::string, create_call >::const_iterator Const_ElemItr;

		typedef std::list< boost::shared_ptr< GuiWindow > > WindowList;
		typedef WindowList::iterator WindowItr;

		WindowList m_windowList;

		std::map< std::string, create_call > m_elementCreate;
		Engine* m_pImplEngine;

	private:
		mutable boost::weak_ptr< GuiWindow > m_lastAddedWindow;
	};
}

}
#endif