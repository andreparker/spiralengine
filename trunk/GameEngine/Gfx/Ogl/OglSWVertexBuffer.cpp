#include <boost/make_shared.hpp>
#include "OglSWVertexBuffer.hpp"
#include "../VertexFormatImpl.hpp"
#include "OglSWVertexBufferResource.hpp"
#include "OglUtility.hpp"

using namespace boost;
using namespace Spiral;

OglSWVertexBuffer* OglSWVertexBuffer::DoClone() const
{
	return NULL;
}

void OglSWVertexBuffer::DoBind()
{
	OglSWVertexBufferResource* resource = static_cast< OglSWVertexBufferResource*>( GetResource().get() );
	OglUtil::EnableArrayPointers( GetVertexType(), GetStride(), resource->GetData() );
}

void OglSWVertexBuffer::DoUnBind()
{
	OglUtil::DisableArrayPointers( GetVertexType() );
}

bool OglSWVertexBuffer::DoCreate( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool /*bManaged*/ )
{
	shared_ptr< Resource > resource = make_shared< OglSWVertexBufferResource >( elementSize * vertexCount );
	SetVertexType( format.GetType() );
	SetResource( resource );
	SetStride( elementSize );

	return bool( resource );
}

OglSWVertexBuffer::OglSWVertexBuffer()
{

}