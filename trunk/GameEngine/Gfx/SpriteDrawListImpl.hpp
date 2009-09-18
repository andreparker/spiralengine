#ifndef SPRITE_DRAW_LIST_IMPL_HPP
#define SPRITE_DRAW_LIST_IMPL_HPP

#include <list>

namespace Spiral
{

	class Sprite;
namespace Impl
{

	struct SpriteDrawList
	{
		SpriteDrawList():
	    m_opaqueList(),
		m_alphaList(),
		m_bBuildList(true)
		{}

		bool NeedsBuild()const
		{
			return m_bBuildList;
		}

		void SetBuildList( bool enable )
		{
			m_bBuildList = enable;
		}

		void AddSpriteList( const std::list< Sprite* >& spriteList );
		void ClearList()
		{
			m_alphaList.clear();
			m_opaqueList.clear();
		}

		std::list< Sprite* > m_opaqueList;
		std::list< Sprite* > m_alphaList;
	private:
		void AddSprite( Sprite* sprite );
		bool m_bBuildList;
	};
}

typedef Impl::SpriteDrawList SpriteDrawList;
}
#endif