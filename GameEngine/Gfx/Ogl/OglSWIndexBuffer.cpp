#include <boost/make_shared.hpp>

#include "../IndexFormatImpl.hpp"
#include "OglSWIndexBuffer.hpp"
#include "OglSWIndexBufferResource.hpp"
#include "OglUtility.hpp"

using namespace Spiral;
using namespace boost;

OglSWIndexBuffer::OglSWIndexBuffer()
{

}

OglSWIndexBuffer* OglSWIndexBuffer::DoClone() const
{
	return NULL;
}

void OglSWIndexBuffer::DoBind()
{
	OglSWIndexBufferResource* resource = static_cast<OglSWIndexBufferResource*>( GetResource().get() );
	OglUtil::SetIndexBufferPointer( resource->GetData(), GetType() );
}

void OglSWIndexBuffer::DoUnBind()
{
	OglUtil::SetIndexBufferPointer( NULL, GetType() ); // reset
}

bool OglSWIndexBuffer::DoCreate( const IndexFormat& format, int32_t indexCount, bool bManaged )
{
	shared_ptr< Resource > resource = make_shared< OglSWIndexBufferResource >( indexCount * format.GetSize() );
	SetResource( resource );
	
	return bool( resource );
}