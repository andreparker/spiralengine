#include <algorithm>
#include <boost/bind.hpp>


#include "SpriteDrawListImpl.hpp"
#include "Sprite.hpp"

#include "../Core/GeneralException.hpp"

using namespace Spiral;


void Impl::SpriteDrawList::AddSprite( Sprite* sprite )
{
	if( NULL != sprite )
	{
		if( sprite->hasAlpha() )
		{
			m_alphaList.push_back( sprite );
		}else
		{
			m_opaqueList.push_back( sprite );
		}
	}else
	{
		THROW_GENERAL_EXCEPTION( "SpriteDrawList::AddSprite - Null sprite added!" );
	}

}

void Impl::SpriteDrawList::AddSpriteList( const std::list< Sprite* >& spriteList )
{
	std::for_each( spriteList.begin(), spriteList.end(), boost::bind( &SpriteDrawList::AddSprite, this, _1 ) );
}