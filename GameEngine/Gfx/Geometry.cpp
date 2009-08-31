#include "Geometry.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexFormatImpl.hpp"

using namespace Spiral;
using namespace boost;

void Geometry::Bind()
{
	if( m_vertexBuffer )
	{
		m_vertexBuffer->Bind();
	}

	if( m_indexBuffer )
	{
		m_indexBuffer->Bind();
	}
}

void Geometry::UnBind()
{
	if( m_vertexBuffer )
	{
		m_vertexBuffer->UnBind();
	}

	if( m_indexBuffer )
	{
		m_indexBuffer->UnBind();
	}
}

Geometry::Geometry():
m_vertexBuffer(),
m_indexBuffer()
{

}

Geometry::~Geometry()
{

}

bool Geometry::CreateHWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged )
{
	return DoCreateHWVertexBuffer( format, elementSize, vertexCount, bManaged );
}

bool Geometry::CreateSWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged )
{
	return DoCreateSWVertexBuffer( format, elementSize, vertexCount, bManaged );
}

void Geometry::ReleaseVertexBuffer()
{
	m_vertexBuffer.reset();
}

void Geometry::ReleaseIndexBuffer()
{
	m_indexBuffer.reset();
}

void Geometry::ReleaseBuffers()
{
	ReleaseVertexBuffer();
	ReleaseIndexBuffer();
}

bool Geometry::CreateSWIndexBuffer( const IndexFormat& format, int32_t indexCount, bool bManaged )
{
	return DoCreateSWIndexBuffer( format, indexCount, bManaged );
}

bool Geometry::CreateHWIndexBuffer( const IndexFormat& format, int32_t indexCount, bool bManaged )
{
	return DoCreateHWIndexBuffer( format, indexCount, bManaged );
}