#ifndef SPRITE_LAYER_IMPL_HPP
#define SPRITE_LAYER_IMPL_HPP

#include <list>

namespace Spiral
{

	class Sprite;
namespace Impl
{
	struct SpriteLayer
	{
		SpriteLayer():
		m_spriteList(),
		m_enable(false)
		{}

		typedef std::list< Sprite* > SpriteList;
		typedef std::list< Sprite* >::iterator sprite_itr;
		typedef std::list< Sprite* >::const_iterator const_sprite_itr;
		SpriteList m_spriteList;
		bool m_enable;
	};
}

typedef Impl::SpriteLayer SpriteLayer;

}

#endif