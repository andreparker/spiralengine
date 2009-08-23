#include "Texture.hpp"

using namespace Spiral;

Texture::Texture():
m_width(0),m_height(0),
m_bitDepth(0),m_resource()
{

}

Texture::~Texture()
{

}

Cloneable* Texture::DoClone()const
{
	return NULL;	
}
