#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <map>

#include "../../Core/Rect.hpp"
#include "../../Math/Math.hpp"


namespace Spiral
{

	class Texture;
namespace GUI
{
	struct mouse_position;
	class GuiManager;

	class GuiWindow : boost::noncopyable
	{
	public:
		friend class GuiManager;

		typedef boost::function< void( boost::int32_t, GuiWindow*, const boost::any& ) > WindowEventHandler;
		GuiWindow( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords,const boost::shared_ptr< Texture >& texture, bool bAlpha );
		GuiWindow( const Math::SpVector2r& position, const Rect< SpReal >& rect, const boost::shared_ptr< Texture >& texture, bool bAlpha );
		virtual ~GuiWindow(){}

		bool hasAlpha()const
		{
			return m_alpha;
		}

		const Rect< SpReal >& GetRect()const
		{
			return m_rect;
		}

		const Rect< SpReal >& GetTextCoords()const
		{
			return m_textCoords;
		}

		const boost::shared_ptr< Texture >& GetTexture()const
		{
			return m_texture;
		}

		const Math::SpVector2r& GetLocalPosition()const
		{
			return m_localPosition;
		}

		Math::SpVector2r WorldToLocal( SpReal x, SpReal y )
		{
			return Math::make_vector< SpReal >( x - m_worldPosition[0], y - m_worldPosition[1] );
		}

		void SetLocalPosition( const Math::SpVector2r& position )
		{
			m_localPosition = position;

			// update my world and children positions
			if( m_parent )
			{
				
				UpdatePositions( m_parent->GetWorldPosition() );
			}else
			{
				UpdatePositions();
			}
		}

		bool hasFocus()const
		{
			return m_hasFocus;
		}

		boost::uint32_t GetID()const
		{
			return m_windowId;
		}

		void AllowFocus( bool allow = true )
		{
			m_allowFocus = allow;
		}

		bool FocusAllowed()const
		{
			return m_allowFocus;
		}

		/*!
		   @function  AddChild
		   @brief     adds a child window
		   @return    void
		   @param     const boost::shared_ptr< GuiWindow > & window
		*/
		void AddChild( const boost::shared_ptr< GuiWindow >& window );
		GuiWindow* GetChild( boost::uint32_t window_id )const;

		/*!
		   @function  RemoveChild
		   @brief     removes a window
		   @return    void
		   @param     const boost::shared_ptr< GuiWindow > & window
		*/
		void RemoveChild( const boost::shared_ptr< GuiWindow >& window );
		void RemoveChild( boost::uint32_t window_id );

		/*!
		   @function  Create
		   @brief     creates a window
		   @return    boost::shared_ptr< GuiWindow >
		   @param     const Math::SpVector2r & position
		   @param     const Rect< SpReal > & rect
		   @param     const boost::shared_ptr< Texture > & texture
		   @param     bool bAlpha
		*/
		static boost::shared_ptr< GuiWindow > Create( const Math::SpVector2r& position, const Rect< SpReal >& rect, const boost::shared_ptr< Texture >& texture, bool bAlpha );
		static boost::shared_ptr< GuiWindow > Create( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords, boost::shared_ptr< Texture >& texture, bool bAlpha );


		/*!
		   @function  Show
		   @brief     shows/hides the window
		   @return    void
		   @param     bool bshow
		*/
		void Show( bool bshow = true )
		{
			m_show = bshow;
		}

		/*!
		   @function  ConnectHandler
		   @brief     binds function callbacks to events
		   @return    void
		   @param     boost::int32_t eventId
		   @param     const WindowEventHandler & handler
		*/
		void ConnectHandler( boost::int32_t eventId, const WindowEventHandler& handler );
		void DisConnectHandler( boost::int32_t eventId );

		GuiWindow* GetParent()const
		{
			return m_parent;
		}

		MAKE_ALIGNED_NEW
	protected:


		void SetParent( GuiWindow* parent )
		{
			m_parent = parent;
		}

		const Math::SpVector2r& GetWorldPosition()const
		{
			return m_worldPosition;
		}

		/*!
		   @function  FindTopMostChild
		   @brief     finds the top most child that the coordinates lie in
		   @return    GuiWindow*
		   @param     SpReal x
		   @param     SpReal y
		*/
		GuiWindow* FindTopMostChild( SpReal x, SpReal y )const;

		/*!
		   @function  ProcessMouseEvent
		   @brief     processes a event and calls appropriate handlers
		   @return    void
		   @param     boost::int32_t eventId
		   @param     const mouse_position & pos
		*/
		void ProcessMouseEvent( boost::int32_t eventId, const mouse_position& pos );

		/*!
		   @function  ProcessEvent
		   @brief     process all events
		   @return    void
		   @param     boost::int32_t eventId
		   @param     const boost::any & data
		*/
		void ProcessEvent( boost::int32_t eventId, const boost::any& data );

		/*!
		   @function  CallHandler
		   @brief     calls a event handler registered with ConnectHandler
		   @return    void
		   @param     boost::int32_t eventId
		   @param     GuiWindow * window
		   @param     const boost::any & data
		*/
		void CallHandler( boost::int32_t eventId, GuiWindow* window, const boost::any& data );

		void SetTexture( const boost::shared_ptr< Texture >& texture )
		{
			m_texture = texture;
		}

		void SetTexCoords( const Rect< SpReal >& rt )
		{
			m_textCoords = rt;
		}

		void SetRect( const Rect< SpReal >& rt )
		{
			m_rect = rt;
		}

		void UpdatePositions()
		{
			UpdatePositions( Math::SpVector2r(0.0f,0.0f) );
		}

	private:

		void UpdatePositions( const Math::SpVector2r& worldPosition );

		bool IsDirty()const
		{
			return m_dirty;
		}

		/*!
		   @function  SetFocus
		   @brief     sets focus to the window if x,y lies with in its bounds
		   @return    bool
		   @param     SpReal x
		   @param     SpReal y
		*/
		bool SetFocus( SpReal x, SpReal y );

		virtual void MouseDown( const mouse_position& pos );
		virtual void MouseUp( const mouse_position& pos );
		virtual void MouseHover( const mouse_position& pos );
		virtual void CharInput( boost::uint32_t char_ );

		bool ContainsPoint( SpReal x, SpReal y )const
		{
			SpReal localx = x - m_worldPosition[0];
			SpReal localy = y - m_worldPosition[1];

			return m_rect.Contains( localx, localy );
		}
		
		/*!
		   @function  ResetWindow
		   @brief     resets the window to it defualt state before focus
		   @return    void
		*/
		virtual void ResetWindow();
	private:
		static boost::int32_t window_ID;

		typedef std::list< boost::shared_ptr< GuiWindow > >::iterator WindowItr;
		typedef std::list< boost::shared_ptr< GuiWindow > >::const_iterator Const_WindowItr;
		typedef std::multimap< boost::int32_t, WindowEventHandler >::iterator handleItr;

		bool IsVisible()const
		{
			return m_show;
		}

		Math::SpVector2r m_localPosition;
		Math::SpVector2r m_worldPosition;
		Rect< SpReal > m_rect;
		Rect< SpReal > m_textCoords;
		boost::shared_ptr< Texture > m_texture;
		typedef std::list< boost::shared_ptr< GuiWindow > >::const_iterator const_window_itr;
		typedef std::list< boost::shared_ptr< GuiWindow > >::iterator window_itr;
		std::list< boost::shared_ptr< GuiWindow > > m_children;
		boost::uint32_t m_windowId;
		std::multimap< boost::int32_t, WindowEventHandler > m_eventHandlers; 

		bool m_hasFocus;
		bool m_alpha;
		bool m_dirty;		///< used to update world positioning
		bool m_show;		///< shows the window
		bool m_allowFocus;  ///< enable/disables focus on the window
		GuiWindow* m_parent;
	};
}
}
#endif