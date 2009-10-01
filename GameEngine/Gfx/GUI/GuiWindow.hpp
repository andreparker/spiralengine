#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include <list>
#include "../../Core/Rect.hpp"
#include "../../Math/Math.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

namespace Spiral
{

	class Texture;
namespace GUI
{
	class GuiManager;

	class GuiWindow
	{
	public:
		friend class GuiManager;

		GuiWindow( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords,const boost::shared_ptr< Texture >& texture, bool bAlpha );
		GuiWindow( const Math::SpVector2r& position, const Rect< SpReal >& rect, const boost::shared_ptr< Texture >& texture, bool bAlpha );

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

		void SetLocalPosition( const Math::SpVector2r& position )
		{
			m_localPosition = position;
			m_dirty = true;
		}

		bool hasFocus()const
		{
			return m_hasFocus;
		}

		boost::uint32_t GetID()const
		{
			return m_windowId;
		}

		void AddChild( const boost::shared_ptr< GuiWindow >& window );
		void RemoveChild( const boost::shared_ptr< GuiWindow >& window );
		void RemoveChild( boost::uint32_t window_id );

		static boost::shared_ptr< GuiWindow > Create( const Math::SpVector2r& position, const Rect< SpReal >& rect, const boost::shared_ptr< Texture >& texture, bool bAlpha );
		static boost::shared_ptr< GuiWindow > Create( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords, boost::shared_ptr< Texture >& texture, bool bAlpha );


		void Show( bool bshow )
		{
			m_show = bshow;
		}

		MAKE_ALIGNED_NEW
	private:

		void UpdatePositions( const Math::SpVector2r& worldPosition );
		void UpdatePositions()
		{
			UpdatePositions( Math::SpVector2r(0.0f,0.0f) );
		}

		const Math::SpVector2r& GetWorldPosition()const
		{
			return m_worldPosition;
		}

		bool IsDirty()const
		{
			return m_dirty;
		}

		bool SetFocus( SpReal x, SpReal y );
	private:
		bool IsVisible()const
		{
			return m_show;
		}

		Math::SpVector2r m_localPosition;
		Math::SpVector2r m_worldPosition;
		Rect< SpReal > m_rect;
		Rect< SpReal > m_textCoords;
		boost::shared_ptr< Texture > m_texture;
		typedef std::list< boost::shared_ptr< GuiWindow > >::iterator WindowItr;
		std::list< boost::shared_ptr< GuiWindow > > m_children;
		boost::uint32_t m_windowId;
		bool m_hasFocus;
		bool m_alpha;
		bool m_dirty;		//< used to update world positioning
		bool m_show;
	};
}
}
#endif