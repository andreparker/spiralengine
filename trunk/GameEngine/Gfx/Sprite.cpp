#include "Sprite.hpp"


using namespace Spiral;

Sprite::Sprite( boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo ):
m_texture( texture ),
m_buffer(),
m_spriteTexCoords( spriteTexCoords ),
m_position( 0.0f, 0.0f ),
m_angle( 0.0f ),
m_width( spriteInfo.right ),
m_height( spriteInfo.bottom ),
m_alphaBlend( false ),
m_additiveBlend( false ),
m_rotOffsetX( spriteInfo.left ),m_rotOffsetY( spriteInfo.top )
{

}