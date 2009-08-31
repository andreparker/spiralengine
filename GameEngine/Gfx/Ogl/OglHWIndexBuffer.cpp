#include "OglHWIndexBuffer.hpp"
#include "OglHWIndexBufferResource.hpp"
#include "OglUtility.hpp"
#include "../IndexFormatImpl.hpp"
#include <boost/make_shared.hpp>

using namespace Spiral;
using namespace boost;

OglHWIndexBuffer::OglHWIndexBuffer()
{

}

void OglHWIndexBuffer::DoBind()
{
	OglHWIndexBufferResource* resource = static_cast<OglHWIndexBufferResource*>( GetResource().get() );
	resource->Bind();
	OglUtil::SetIndexBufferPointer( NULL, GetType() ); // pointer is in gl

}

void OglHWIndexBuffer::DoUnBind()
{
	OglHWIndexBufferResource* resource = static_cast<OglHWIndexBufferResource*>( GetResource().get() );
	resource->UnBind();
}

OglHWIndexBuffer* OglHWIndexBuffer::DoClone() const
{
	return NULL;
}

bool OglHWIndexBuffer::DoCreate( const IndexFormat& format, boost::int32_t indexCount, bool /*bManaged*/ )
{
	shared_ptr< Resource > resource = make_shared< OglHWIndexBufferResource >( indexCount * format.GetSize() );
	SetResource( resource );

	return bool( resource );
}