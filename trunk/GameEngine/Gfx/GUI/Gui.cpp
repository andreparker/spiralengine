#include "Gui.hpp"
#include "../../Core/Engine.hpp"
#include "../GfxDriver.hpp"
#include "../GfxImpl.hpp"
#include "../RenderState.hpp"
#include "../../Core/MouseEvent.hpp"
#include "../../Core/Events.hpp"
#include "../Texture.hpp"
#include "../Color.hpp"
#include "../Font.hpp"
#include "../../Math/Math.hpp"
#include "GuiWindow.hpp"
#include "GuiWindowEvents.hpp"
#include "../../Core/File.hpp"
#include "../../Core/FileManager.hpp"
#include "../../Core/Log.hpp"
#include "../../Script/ScriptManager.hpp"
#include "../../Core/GeneralException.hpp"
#include "../../Core/UpdateQueue.hpp"

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/assert.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/type_traits.hpp>


#include "GuiButton.hpp"
#include "GuiSlider.hpp"
#include "GuiText.hpp"
#include "GuiEditBox.hpp"
#include "GuiCheckBox.hpp"
#include "GuiFrame.hpp"

using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;
using namespace boost::property_tree;

namespace
{
	// key words
	const std::string kKwElementFile = "eFile";
	const std::string kKwElementExt  = "eClassEx";
	const std::string kKwLayout      = "layout";
	const std::string kKwTexture     = "texture";
	const std::string kKwElement     = "eClass";
	const std::string kKwPosition    = "position";
	const std::string kKwWindowRect  = "window_rect";
	const std::string kKwTextureRect = "texture_rect";
	const std::string kKwWindowName  = "name";
	const std::string kKwScriptName  = "luaFile";
	const std::string kKwWindowAlpha = "alpha";
	const std::string kKwClipChild   = "clipChildren";
	const std::string kKwAllowFocus  = "allowFocus";
	const std::string kKwShowWindow  = "show";

	// button
	const std::string kKwOnButtonPress = "OnButtonPress";
}


GuiManager::GuiManager( Engine* engine ):
m_windowList(),
m_pImplEngine( engine ),
m_lastAddedWindow()
{
	m_elementCreate = assign::map_list_of
	( "window", bind( &GuiManager::Create_Window_From_Tree, this, _1, _2 ) )
	( "frame" , bind( &GuiManager::Create_Frame_From_Tree, this, _1, _2 ) )
	( "editBox", bind( &GuiManager::Create_EditBox_From_Tree, this, _1, _2 ) )
	( "slider", bind( &GuiManager::Create_Slider_From_Tree, this, _1, _2 ) )
	( "button", bind( &GuiManager::Create_Button_From_Tree, this, _1, _2 ) )
	( "textBox", bind( &GuiManager::Create_TextBox_From_Tree, this, _1, _2 ) )
	( "scrollWindow", bind( &GuiManager::Create_Scroll_From_Tree, this, _1, _2 ) )
	( "checkBox", bind( &GuiManager::Create_CheckBox_From_Tree, this, _1, _2 ) );
}



void GuiManager::AddElement( const boost::shared_ptr< GuiWindow >& window )
{
	m_windowList.push_back( window );
}

void GuiManager::RemoveElement( const boost::shared_ptr< GuiWindow >& window )
{
	m_windowList.remove( window );
}

void GuiManager::TraverseRender( const boost::shared_ptr< GfxDriver >& gfxDrvier, const boost::shared_ptr< GuiWindow >& window )const
{
	if( window->IsVisible() )
	{
		if( window->GetTexture() )
		{
			gfxDrvier->Bind( window->GetTexture() );
			if( window->hasAlpha() )
			{
				gfxDrvier->SetState( RenderState::Blend( RenderState::RS_TRUE ) );
				gfxDrvier->Set( BlendMode_t::SrcAlpha() );
				gfxDrvier->Draw( window->GetWorldPosition(), window->GetRect(), window->GetTextCoords() );
				gfxDrvier->SetState( RenderState::Blend( RenderState::RS_FALSE ) );
			}else
			{
				gfxDrvier->Draw( window->GetWorldPosition(), window->GetRect(), window->GetTextCoords() );
			}
			
		}

		for( const_gui_window_itr itr = window->m_children.begin(); itr != window->m_children.end(); ++itr )
		{
			if( window->ClipChildren() )
			{
				Rect<SpReal> rct = window->GetRect();
				Math::Vector2f pos,widthHeight;
				
				pos = window->GetWorldPosition();
				
				widthHeight[0] = rct.right + pos[0];
				widthHeight[1] = rct.bottom + pos[1];

				m_pImplEngine->WorldToScreen( pos, pos );
				m_pImplEngine->WorldToScreen( widthHeight, widthHeight );
				widthHeight -= pos;
				gfxDrvier->SetClipRect( static_cast<boost::int32_t>(pos[0] + 0.99f), 
					                    static_cast<boost::int32_t>(pos[1] + 0.99f),
										static_cast<boost::int32_t>(widthHeight[0] + 0.99f),
										static_cast<boost::int32_t>(widthHeight[1] + 0.99f) );
				gfxDrvier->SetState( RenderState::ClipRect( RenderState::RS_TRUE ) );

			}
			TraverseRender( gfxDrvier, *itr );
			if( window->ClipChildren() )
			{
				gfxDrvier->SetState( RenderState::ClipRect( RenderState::RS_FALSE ) );
			}
		}
	}
}

void GuiManager::Input( const Event& inputEvent, const boost::any& data )
{
	if( inputEvent.hasCat( Catagory_Mouse::value ) )
	{
		MouseEvent event_data = boost::any_cast< MouseEvent >( data );
		mouse_position position( static_cast<SpReal>(event_data.pos.x), static_cast<SpReal>(event_data.pos.y) );
		HandleMouseInput( inputEvent, position );
		PlaceFocusWindow();
	}else if( inputEvent.hasCat( Catagory_KeyBoard::value ) )
	{
		if( inputEvent.IsCat( Catagory_KeyBoard_Char::value ) )
		{
			HandleCharInput( inputEvent, boost::any_cast< uint32_t >( data ) );
		}
		
	}
}

void GuiManager::HandleMouseInput( const Event& inputEvent, const mouse_position& position )
{
	if( inputEvent.IsCat( Catagory_Mouse_MouseDown::value ) )
	{
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseDown, _1, boost::cref(position) ) );
		/*/
		BOOST_FOREACH( shared_ptr<GuiWindow>& window, m_windowList )
		{
			window->MouseDown( position );
		}
		//*/
	}else
		if( inputEvent.IsCat( Catagory_Mouse_Up::value ) )
	{
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseUp, _1, boost::cref(position) ) );
		/*
		BOOST_FOREACH( shared_ptr<GuiWindow>& window, m_windowList )
		{
			window->MouseDown( position );
		}
		//*/
	}else
		if( inputEvent.IsCat( Catagory_Mouse_Move::value ) )
	{
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseHover, _1, boost::cref(position) ) );
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseMove, _1, boost::cref(position) ) );
	}
}

void GuiManager::HandleCharInput( const Event& inputEvent, boost::uint32_t char_ )
{
	std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::CharInput, _1, char_ ) ) ;
}

void GuiManager::Present( const boost::shared_ptr< GfxDriver >& gfxDrvier )
{
	std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiManager::TraverseRender, this, boost::cref( gfxDrvier ), _1 ) );
}



void GuiManager::Clear()
{
	m_windowList.clear();
	GuiWindow::lastHoverWindow = NULL;
	GuiWindow::lastFocusWindow = NULL;
}

boost::shared_ptr< GuiButton > GuiManager::Make_DefButton( SpReal posX, SpReal posY, SpReal width, SpReal height )
{
	shared_ptr< Texture > button_texture = m_pImplEngine->LoadTexture( "Data/GUI/def_button.png", "Button_texture" );
	shared_ptr< GuiButton > button = make_shared< GuiButton >( Math::make_vector( posX,posY), Rect< SpReal >( 0, width, height, 0 ), 
		Rect< SpReal >( 0.0f, 0.5f, 1.0f, 0.0f ), button_texture, true );
	return button;
}

boost::shared_ptr< GuiWindow > GuiManager::Make_DefWindow( SpReal posX, SpReal posY, SpReal width, SpReal height )
{
	shared_ptr< Texture > window_texture = m_pImplEngine->LoadTexture( "Data/GUI/def_window_new.png", "Window_texture" );
	shared_ptr< GuiWindow > window = make_shared< GUI::GuiWindow >( Math::make_vector( posX, posY ), Rect< SpReal >( 0, width, height, 0 ),
		Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), window_texture, true );
	return window;
}

boost::shared_ptr< GuiCheckBox > GuiManager::Make_DefCheckBox( SpReal posX, SpReal posY, SpReal width, SpReal height )
{
	shared_ptr< Texture > check_boxTexture = m_pImplEngine->LoadTexture( "Data/GUI/def_check.png", "check_texture" );
	shared_ptr< GuiCheckBox > check = make_shared< GuiCheckBox >( Math::make_vector( posX, posY ),
		Rect< SpReal >( 0.0f, width, height, 0.0f ), Rect< SpReal >( 0.0f, 0.5f, 1.0f, 0.0f ), Rect< SpReal >( 0.5f, 1.0f, 1.0f, 0.0f ),
		check_boxTexture,check_boxTexture,true,true );

	return check;
}

boost::shared_ptr< GuiText > GuiManager::Make_DefTextBox( SpReal posX, SpReal posY, const Rgba& fontColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxAllowedChar, const wString& text )
{
	shared_ptr< GuiText > guiText = make_shared< GuiText >( Math::make_vector<SpReal>(posX, posY),m_pImplEngine->GetGfxDriver(),fontColor,maxAllowedChar,font,text,true );
	return guiText;
}

boost::shared_ptr< GuiSlider > GuiManager::Make_DefSlider( SpReal posX, SpReal posY, SpReal width, SpReal height, boost::uint32_t sliderSize, const GuiSliderDir& dir )
{
	shared_ptr< Texture > slider_bgTexture;
	shared_ptr< Texture > slider_texture = m_pImplEngine->LoadTexture( "Data/GUI/def_slider.png", "slider_texture" );

	Rect<SpReal> txcoords0,txcoords1;
	
	txcoords0 = Rect<SpReal>( 0.0f,1.0f,1.0f,0.0f );
	txcoords1 = Rect<SpReal>( 0.0f,1.0f,1.0f,0.0f );

	if( GuiSliderDir::Vertical == dir.GetDir() )
	{
		slider_bgTexture = m_pImplEngine->LoadTexture( "Data/GUI/def_slider_back_v.png", "slider_bk_vert" );
	}else
	{
		slider_bgTexture = m_pImplEngine->LoadTexture( "Data/GUI/def_slider_back.png", "slider_bk_horz" );
	}

	shared_ptr< GuiSlider > guiSlider( new GuiSlider( Math::make_vector( posX, posY ), Rect<SpReal>( 0.0f, width, height, 0.0f ), 
		txcoords0, txcoords1,sliderSize, slider_bgTexture,
		slider_texture, dir, true, true ) );

	return guiSlider;
}

boost::shared_ptr< GuiEditBox > GuiManager::Make_DefEditBox( SpReal posX, SpReal posY, const Rgba& bkColor, const Rgba& fontColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const wString& text )
{
	shared_ptr< GuiEditBox > editbox = make_shared< GuiEditBox >( Math::make_vector( posX,posY ), m_pImplEngine->GetGfxDriver(), 
		bkColor, fontColor, font, maxCharLen, text );

	return editbox;
}

boost::shared_ptr< GuiFrame > GuiManager::Make_DefFrame( SpReal posX, SpReal posY, SpReal width, SpReal height )
{
	shared_ptr< Texture > window_texture = m_pImplEngine->LoadTexture( "Data/GUI/def_frame_window.png", "Frame_window_texture" );
	shared_ptr< GuiFrame > frame = make_shared< GuiFrame >( Math::make_vector( posX, posY ), Rect< SpReal >( 0, width, height, 0 ),
		Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), window_texture, true );
	return frame;
}

namespace
{
	bool ToBoolean( const std::string& str )
	{
		BOOST_ASSERT( str == "true" || str == "false" );
		return bool( "true" == str ? true : ( "false" == str ? false : false ) );
	}

	void ExtractBounds( const ptree& tree, SpReal* bounds )
	{
		ptree::const_iterator leftItr   = tree.find( "left" );
		ptree::const_iterator rightItr  = tree.find( "right" );
		ptree::const_iterator topItr    = tree.find( "top" );
		ptree::const_iterator bottomItr = tree.find( "bottom" );

		*bounds++ = leftItr->second.get_value( 0.0f );
		*bounds++ = rightItr->second.get_value( 0.0f );
		*bounds++ = topItr->second.get_value( 0.0f );
		*bounds   = bottomItr->second.get_value( 0.0f );
	}

	shared_ptr< Texture > ExtractTexture( Engine* engine, const ptree& tree )
	{
		shared_ptr< Texture > newTexture;
		ptree::const_iterator fileName = tree.find( "fileName" );
		ptree::const_iterator tagName  = tree.find( "tagName"  );

		if( fileName != tree.end() && tagName != tree.end() )
		{
			newTexture = engine->LoadTexture( fileName->second.data(), tagName->second.data() );
		}else if( tagName != tree.end() )
		{
			newTexture = engine->GetTexture( tagName->second.data() );
		}

		return newTexture;
	}

	void ExtractScript( Engine* engine, const ptree& tree )
	{
		ptree::const_iterator fileName = tree.find( "fileName" );
		ptree::const_iterator tagName  = tree.find( "tagName"  );

		boost::shared_ptr< IFile > scriptFile;
		if( FileManager::instance().getFile( fileName->second.data(), scriptFile ) )
		{
			engine->GetScriptManager()->LoadScript( scriptFile, tagName->second.data().c_str() );
		}else
		{
			LOG_E( "^yExtractScript: ^rError opening script %1%\n", fileName->second.data() );
		}
	}

	std::string currentlayoutFile = "";
	void SetLayoutFile( const std::string& fileName )
	{
		currentlayoutFile = fileName;
	}

	void ExtractPosition( const ptree& tree, SpReal* position )
	{
		ptree::const_iterator xItr = tree.find( "x" );
		if( xItr != tree.end() )
		{
			*position++ = xItr->second.get_value( 0.0f );
		}

		ptree::const_iterator yItr = tree.find( "y" );
		if( yItr != tree.end() )
		{
			*position++ = yItr->second.get_value( 0.0f );
		}

		ptree::const_iterator zItr = tree.find( "z" );
		if( zItr != tree.end() )
		{
			*position++ = zItr->second.get_value( 0.0f );
		}
	}

	void ReadInfo( const boost::shared_ptr<IFile>& file, ptree& tree )
	{
		boost::shared_ptr< std::istream > pStream = file->GetStream();
		try
		{
			info_parser::read_info( *pStream, tree );

		}catch( std::exception& e )
		{
			std::string nl = "\n";
			THROW_GENERAL_EXCEPTION( "Error in layout file - " + currentlayoutFile + nl + e.what() );
		}
	}
}

void GuiManager::BaseWindowAttributes( ptree::const_iterator& itr, const boost::shared_ptr< GuiWindow >& newWindow,
									   const std::string& parentPath )const
{
	SpReal bounds[4];
	if( IsElement( itr->first ) )
	{
		newWindow->AddChild( Create_Element( itr->second.data(), parentPath + "." + itr->first, itr->second ) );
	}
	else if( itr->first == kKwPosition )
	{
		SpReal position[2];
		ExtractPosition( itr->second, position );
		newWindow->SetLocalPosition( Math::make_vector( position[0], position[1] ) );
	}else if( itr->first == kKwWindowRect )
	{
		ExtractBounds( itr->second, bounds );
		newWindow->SetRect( Rect<SpReal>( bounds[0],bounds[1], bounds[3], bounds[2] ) );
	}else if( itr->first == "texture_rect" )
	{
		ExtractBounds( itr->second, bounds );
		newWindow->SetTexCoords( Rect<SpReal>( bounds[0],bounds[1], bounds[3], bounds[2] ) );
	}else if( itr->first == kKwTexture )
	{
		newWindow->SetTexture( ExtractTexture( const_cast< Engine* >( m_pImplEngine ), itr->second ) );
	}else if( itr->first == kKwWindowAlpha )
	{
		newWindow->SetAlphaBlend( ToBoolean( itr->second.data() ) );
	}else if( itr->first == kKwAllowFocus )
	{
		newWindow->AllowFocus( ToBoolean( itr->second.data() ) );
	}else if( itr->first == kKwShowWindow )
	{
		newWindow->Show( ToBoolean( itr->second.data()  ) );
	}else if( itr->first == kKwElementFile )
	{
		shared_ptr< IFile > elementFile;
		SetLayoutFile( itr->second.data() );
		if( FileManager::instance().getFile( itr->second.data(), elementFile ) )
		{
			LoadSubLayout( newWindow, elementFile );
			elementFile->Close();

			// check for overrides of attributes and if a child was added
			OverRideAttributes(parentPath, itr);
		}
	}else if( itr->first == kKwScriptName )
	{
		ExtractScript( const_cast< Engine* >( m_pImplEngine ), itr->second );
	}else if( itr->first == kKwWindowName )
	{
		newWindow->SetName( itr->second.data() );
	}
}

void GuiManager::OverRideAttributes( const std::string& parentPath, ptree::const_iterator& itr )const
{
	if( m_lastAddedWindow.expired() == false )
	{
		std::string path =  parentPath + "." + kKwElementFile;
		shared_ptr< GuiWindow > lastChild( m_lastAddedWindow );
		const ptree& subTree = itr->second;
		// loop through attribute overrides
		for( ptree::const_iterator subItr = subTree.begin(); subItr != subTree.end(); ++subItr )
		{
			BaseWindowAttributes( subItr, lastChild, path );
		}
	}
}

bool GuiManager::LoadLayoutImpl( const boost::shared_ptr< IFile >& layoutFile, const boost::function< void( const boost::shared_ptr<GuiWindow>& ) >& storeFunction ) const
{
	bool isLoaded = true;
	ptree tree;

	ReadInfo( layoutFile, tree );
	ptree::iterator layoutItr = tree.find( kKwLayout );

	if( layoutItr != tree.end() )
	{
		ptree& layoutVal = layoutItr->second;
		for( ptree::iterator itr = layoutVal.begin(); itr != layoutVal.end(); ++itr )
		{
			if( IsElement( itr->first ) )
			{
				shared_ptr< GuiWindow > newWindow = Create_Element( itr->second.data(), 
					                                                layoutItr->first + "." + itr->first, itr->second );
				// save a ref to the window for attribute overriding
				m_lastAddedWindow = newWindow;

				if( newWindow )
				{
					storeFunction( newWindow );
				}
			}else if( itr->first == kKwElementFile || itr->first == kKwElementExt )
			{
				shared_ptr< IFile > elementFile;
				SetLayoutFile( itr->second.data() );
				if( FileManager::instance().getFile( itr->second.data(), elementFile ) )
				{
					const_cast<GuiManager*>(this)->LoadLayout( elementFile );
					elementFile->Close();
					OverRideAttributes( layoutItr->first + "." + itr->first, itr );
				}
			}else if( itr->first == kKwTexture )
			{
				ExtractTexture( const_cast<Engine*>( m_pImplEngine ), itr->second );
			}else if( itr->first == kKwScriptName )
			{
				ExtractScript( const_cast< Engine* >( m_pImplEngine ), itr->second );
			}
		}
	}

	return isLoaded;
}

bool GuiManager::LoadLayout( const boost::shared_ptr<IFile>& layout )
{
	return LoadLayoutImpl( layout, boost::bind( &GuiManager::AddElement, this, _1 ) );
}

bool GuiManager::LoadSubLayout( const boost::shared_ptr< GuiWindow >& window, const boost::shared_ptr< IFile >& layoutFile )const
{
	return LoadLayoutImpl( layoutFile, boost::bind( &GuiWindow::AddChild, cref(window), _1 ) );
}

boost::shared_ptr< GuiWindow > GuiManager::Create_Element( const std::string& type, const std::string& parentPath,
														   const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiWindow > newWindow;

	Const_ElemItr itr = m_elementCreate.find( type );
	if( itr != m_elementCreate.end() )
	{
		newWindow = (*itr).second( parentPath, tree );
	}

	return newWindow;
}

boost::shared_ptr< GuiWindow > GuiManager::Create_Window_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiWindow > newWindow( new GuiWindow() );

	for( ptree::const_iterator itr = tree.begin(); itr != tree.end(); ++itr )
	{
		BaseWindowAttributes( itr, newWindow, parentPath );
	}

	return newWindow;
}

boost::shared_ptr< GuiWindow > GuiManager::Create_Button_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiButton > newWindow( new GuiButton() );

	newWindow->SetGuiManager( const_cast<GuiManager*>(this) );
	for( ptree::const_iterator itr = tree.begin(); itr != tree.end(); ++itr )
	{
		BaseWindowAttributes( itr, newWindow, parentPath );

		if( itr->first == kKwOnButtonPress )
		{
			newWindow->SetButtonPressScript( itr->second.data() );
		}
	}

	return newWindow;
}

boost::shared_ptr< GuiWindow > GuiManager::Create_Frame_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiWindow > newWindow( new GuiFrame() );

	for( ptree::const_iterator itr = tree.begin(); itr != tree.end(); ++itr )
	{
		BaseWindowAttributes( itr, newWindow, parentPath );
	}

	return newWindow;
}

boost::shared_ptr< GuiWindow > GuiManager::Create_EditBox_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiWindow > newWindow;

	return newWindow;
}

boost::shared_ptr< GuiWindow > GuiManager::Create_TextBox_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiWindow > newWindow;

	return newWindow;
}

boost::shared_ptr< GuiWindow > GuiManager::Create_Scroll_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiWindow > newWindow;

	return newWindow;
}

boost::shared_ptr< GuiWindow > GuiManager::Create_Slider_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiWindow > newWindow;

	return newWindow;
}

boost::shared_ptr< GuiWindow > GuiManager::Create_CheckBox_From_Tree( const std::string& parentPath, const boost::property_tree::ptree& tree ) const
{
	boost::shared_ptr< GuiWindow > newWindow;

	return newWindow;
}

namespace boost
{
	bool operator==( const shared_ptr<GuiWindow>& ls, const GuiWindow* rs )
	{
		return bool( ls.get() == rs );
	}
}

void GuiManager::PlaceFocusWindow()
{
	if( GuiWindow::lastFocusWindow != NULL )
	{
		GuiWindow* parent = GuiWindow::lastFocusWindow->GetAncestor();
		if( m_windowList.back().get() != parent )
		{
			WindowItr itr = std::find( m_windowList.begin(), m_windowList.end(), parent );
			
			shared_ptr<GuiWindow> window = *itr;

			m_windowList.erase( itr );
			m_windowList.push_back( window );
		}
	}
}

namespace
{
	
	const GuiWindow* Window_FindWindow( const GuiWindow* window, const std::string& name )
	{
		return window->FindWindow( name );
	}

	GuiWindow* Window_FindWindow( GuiWindow* window, const std::string& name )
	{
		return window->FindWindow( name );
	}

	GuiButton* window_to_button( GuiWindow* window )
	{
		return static_cast<GuiButton*>( window );
	}

	GuiWindow* button_to_window( GuiButton* button )
	{
		return static_cast<GuiWindow*>( button );
	}

	luabind::scope RegisterWindowSub()
	{
		luabind::scope s = luabind::class_< GuiWindow >( "GuiWindow" )
			.def( "FindWindow", (GuiWindow* (GuiWindow::*)(const std::string&))&GuiWindow::FindWindow )
			.def( "FindWindow", (const GuiWindow* (GuiWindow::*)(const std::string&)const)&GuiWindow::FindWindow )
			.def( "GetName", &GuiWindow::GetName )
			.def( "GetAncestor", (GuiWindow*(GuiWindow::*)())&GuiWindow::GetAncestor )
			.def( "GetAncestor", (const GuiWindow*(GuiWindow::*)()const)&GuiWindow::GetAncestor )
			.def( "IsAncestor", &GuiWindow::IsAncestor )
			.def( "SetClipChildren", &GuiWindow::SetClipChildren )
			.def( "ClipChildren", &GuiWindow::ClipChildren	)
			.def( "SetAlphaBlend", &GuiWindow::SetAlphaBlend );

		return s;
	}

	luabind::scope RegisterButtonSub()
	{
		luabind::scope s = luabind::class_< GuiButton, GuiWindow >( "GuiButton" )
			.def( "SetButtonPressScript", &GuiButton::SetButtonPressScript );

		return s;
	}

	luabind::scope RegisterGuiManagerSub()
	{
		luabind::scope s = luabind::class_< GuiManager >( "GuiManager" )
			.def( "Clear", &GuiManager::Delay_Clear )
			.def( "FindWindow", &GuiManager::FindWindow )
			.def( "LoadLayout", &GuiManager::Delay_LoadLayout_Script );

		return s;
	}

	luabind::scope RegisterGuiFreeFuctionsSub()
	{
		luabind::scope s = luabind::namespace_( "Util" )
		[
			luabind::def( "window_to_button", &window_to_button ),
			luabind::def( "button_to_window", &button_to_window ),
			luabind::def( "Window_FindWindow", (GuiWindow*(*)(GuiWindow*,const std::string&))&Window_FindWindow ),
			luabind::def( "Window_FindWindow", (const GuiWindow*(*)(const GuiWindow*,const std::string&))&Window_FindWindow )
		];

		return s;
	}

	luabind::scope RegisterEditBoxSub()
	{
		luabind::scope s = luabind::class_< GuiEditBox, GuiWindow >( "GuiEditBox" )
			.def( "GetText", &GuiEditBox::GetText )
			.def( "SetText", &GuiEditBox::SetText );

		return s;
	}
}

void GuiManager::Register( const boost::shared_ptr<ScriptManager>& scriptMgr )
{
	luabind::scope s = luabind::namespace_( "GUI" )
	[
		RegisterWindowSub(),
		RegisterButtonSub(),
		RegisterGuiManagerSub(),
		RegisterGuiFreeFuctionsSub()
	];

	scriptMgr->RegisterModule( s );

	// set gui manager global var
	luabind::globals( scriptMgr->GetLuaState() )["GuiManager"] = this;
}

GuiWindow* GuiManager::FindWindow( const std::string& name )
{
	GuiWindow* window = NULL;
	for( WindowItr itr = m_windowList.begin(); itr != m_windowList.end(); ++itr )
	{
		window = (*itr)->FindWindow( name );
		if( window ) 
		{
			break;
		}
	}

	return window;
}

void GuiManager::LoadLayout_Script( const std::string& fileName )
{
	shared_ptr< IFile > file;
	if( FileManager::instance().getFile( fileName, file ) )
	{
		LoadLayout( file );
	}
}

void GuiManager::Delay_Clear()
{
	UpdateQueue::instance().Add( boost::bind( &GuiManager::Clear, this ) ); 
}

void GuiManager::Delay_LoadLayout_Script( const std::string& fileName )
{
	UpdateQueue::instance().Add( boost::bind( &GuiManager::LoadLayout_Script, this, fileName ) );
}