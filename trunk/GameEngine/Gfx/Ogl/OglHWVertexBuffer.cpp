#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "../VertexFormatImpl.hpp"
#include "OglHWVertexBufferResource.hpp"
#include "OglHWVertexBuffer.hpp"
#include "OglUtility.hpp"


using namespace Spiral;
using namespace boost;

OglHWVertexBuffer::OglHWVertexBuffer()
{

}

OglHWVertexBuffer* OglHWVertexBuffer::DoClone() const
{
	return NULL;
}

void OglHWVertexBuffer::DoBind()
{
	OglHWVertexBufferResource* resource = static_cast<OglHWVertexBufferResource*>( GetResource().get() );
	resource->Bind();
	OglUtil::EnableArrayPointers( GetVertexType(), GetStride(), NULL );
}

void OglHWVertexBuffer::DoUnBind()
{
	OglHWVertexBufferResource* resource = static_cast<OglHWVertexBufferResource*>( GetResource().get() );
	resource->UnBind();
	OglUtil::DisableArrayPointers( GetVertexType() );
}

bool OglHWVertexBuffer::DoCreate( const VertexFormat& format, int32_t elementSize, int32_t vertexCount, bool /*bManaged*/ )
{
	shared_ptr< Resource > newRes = make_shared< OglHWVertexBufferResource >( elementSize * vertexCount );
	SetResource( newRes );
	SetStride( elementSize );
	SetVertexType( format.GetType() );

	return ( newRes );
}