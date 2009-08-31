#include "OglGeometry.hpp"
#include "../GeometryType.hpp"
#include "OglHWVertexBuffer.hpp"
#include "OglSWVertexBuffer.hpp"
#include "OglHWIndexBuffer.hpp"
#include "OglSWIndexBuffer.hpp"


using namespace Spiral;
using namespace boost;

OglGeometry::OglGeometry( const GeometryType& type )
{
	SetType( type.GetType() );
}

template< class BufferType >
BufferType* CreateVertexBuffer(  const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged )
{
	BufferType* newVertexBuffer = new BufferType;
	bool result = newVertexBuffer->Create( format, elementSize, vertexCount, bManaged );


	if( result == false )
	{
		delete newVertexBuffer;
		newVertexBuffer = NULL;
	}

	return newVertexBuffer;
	
}

template< class BufferType >
BufferType* CreateIndexBuffer(const IndexFormat& format, boost::int32_t indexCount, bool bManaged )
{
	BufferType* newIndexBuffer = new BufferType;
	bool result = newIndexBuffer->Create( format, indexCount, bManaged );


	if( result == false )
	{
		delete newIndexBuffer;
		newIndexBuffer = NULL;
	}

	return newIndexBuffer;
}


bool OglGeometry::DoCreateHWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged )
{
	OglHWVertexBuffer* newVertexBuffer = CreateVertexBuffer<OglHWVertexBuffer>( format, elementSize, vertexCount, bManaged );
	SetVertexBuffer( newVertexBuffer );
	return bool( newVertexBuffer != NULL );
}

bool OglGeometry::DoCreateSWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged )
{
	OglSWVertexBuffer* newVertexBuffer = CreateVertexBuffer<OglSWVertexBuffer>( format, elementSize, vertexCount, bManaged );
	SetVertexBuffer( newVertexBuffer );
	return bool( newVertexBuffer != NULL );
}

bool OglGeometry::DoCreateSWIndexBuffer( const IndexFormat& format, boost::int32_t indexCount, bool bManaged )
{
	OglSWIndexBuffer* newIndexBuffer = CreateIndexBuffer<OglSWIndexBuffer>( format, indexCount, bManaged );
	SetIndexBuffer( newIndexBuffer );
	return bool( newIndexBuffer != NULL );
}

bool OglGeometry::DoCreateHWIndexBuffer( const IndexFormat& format, boost::int32_t indexCount, bool bManaged )
{
	OglHWIndexBuffer* newIndexBuffer = CreateIndexBuffer<OglHWIndexBuffer>( format, indexCount, bManaged );
	SetIndexBuffer( newIndexBuffer );
	return bool( newIndexBuffer != NULL );
}