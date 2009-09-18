#include <boost/make_shared.hpp>

#include "../../../ThirdParty/GLee/GLee.h"
#include "../GfxImpl.hpp"
#include "OglTexture.hpp"
#include "OglTextureResource.hpp"


using namespace Spiral;
using namespace boost;

OglTexture::OglTexture( const TextureInfo_t& info , const boost::int8_t* data ):
Texture( info.width, info.height, info.bitDepth )
{
	shared_ptr< Resource > resource = make_shared< OglTextureResource >( info, data );
	SetResource( resource );
}

void OglTexture::Bind( boost::int32_t /*unit*/ )const
{
	OglTextureResource* res = static_cast< OglTextureResource* >( GetResource().get() );
	//glActiveTextureARB( GL_TEXTURE0_ARB + unit );
	glBindTexture( GL_TEXTURE_2D, static_cast<GLuint>(res->GetTextureID()) );
}

Cloneable* OglTexture::DoClone() const
{
	return NULL;
}

OglTexture::~OglTexture()
{
}