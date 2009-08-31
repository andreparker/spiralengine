#include "VertexBuffer.hpp"
#include "VertexFormatImpl.hpp"

using namespace boost;
using namespace Spiral;

VertexBuffer::~VertexBuffer()
{

}

VertexBuffer::VertexBuffer():
m_type( VertexFormat::VF_INVALID ),
m_resource(),
m_stride(0)
{

}

bool VertexBuffer::Create( const VertexFormat& format, int32_t elementSize, int32_t vertexCount, bool bManaged /*= false */ )
{
	SetVertexCount( vertexCount );
	return DoCreate( format, elementSize, vertexCount, bManaged );
}
