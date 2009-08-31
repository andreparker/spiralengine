#include "IndexBuffer.hpp"
#include "IndexFormatImpl.hpp"

using namespace Spiral;
using namespace boost;

bool IndexBuffer::Create( const IndexFormat& format, boost::int32_t indexCount, bool bManaged /*= false */ )
{
	SetIndexCount( indexCount );
	SetIndexType( format.GetType() );

	return DoCreate( format, indexCount, bManaged );
}

IndexBuffer::IndexBuffer():
m_indexCount(0),
m_type( IndexFormat::IF_INVALID ),
m_resource()
{

}

IndexBuffer::~IndexBuffer()
{

}