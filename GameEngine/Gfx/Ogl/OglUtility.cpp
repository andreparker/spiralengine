#include "../../../ThirdParty/GLee/GLee.h"
#include "OglUtility.hpp"
#include "../VertexFormatImpl.hpp"
#include "../IndexFormatImpl.hpp"
#include "OglGeometry.hpp"
#include "../GeometryType.hpp"
#include "../VertexBuffer.hpp"
#include "../IndexBuffer.hpp"

using namespace boost;
using namespace Spiral;
using namespace Spiral::OglUtil;

// pointer used to communicate with gl
static int8_t* indexBufferPtr = NULL;
static GLenum  indexSize = GL_UNSIGNED_INT;

#define STRIDE( TYPE_, COUNT_ ) ( sizeof(TYPE_) * (COUNT_) )


inline void EnableVertexData( int32_t elementCount, int32_t stride, int8_t* data )
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( elementCount, GL_FLOAT, stride, data );
}

inline void EnableTexCoordData( int32_t elementCount, int32_t stride, int8_t* data )
{
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer( elementCount, GL_FLOAT, stride, data );
}

void DisableArrayPointers( boost::int32_t vertexFormat )
{
	switch( vertexFormat )
	{
	case VertexFormat::VF_V3T2:
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	case VertexFormat::VF_V3:
		glDisableClientState( GL_VERTEX_ARRAY );
	}
}

void EnableArrayPointers( boost::int32_t vertexFormat, boost::int32_t stride, boost::int8_t* data )
{
	if( vertexFormat == VertexFormat::VF_V3 )
	{
		EnableVertexData( 3, stride, data );
	}else if( vertexFormat == VertexFormat::VF_V3T2 )
	{
		EnableVertexData( 3, stride, data );
		EnableTexCoordData( 2, stride, data + STRIDE( float, 3 ) );
	}
}

void SetIndexBufferPointer( boost::int8_t* bufferPtr, boost::int32_t indexType )
{
	indexBufferPtr = bufferPtr;
	if( IndexFormat::IF_16BIT == indexType )
	{
		indexSize = GL_UNSIGNED_SHORT;
	}else if( IndexFormat::IF_32BIT == indexType )
	{
		indexSize = GL_UNSIGNED_INT;
	}
}

int8_t* GetIndexBufferPointer()
{
	return indexBufferPtr;
}

int32_t GetIndexBufferEnum()
{
	return static_cast<int32_t>(indexSize);
}

void Draw( boost::shared_ptr<Geometry>& geometry )
{
	OglGeometry* geometryPtr = static_cast<OglGeometry*>( geometry.get() );

	GLenum primitiveType = static_cast<GLenum>( geometryPtr->GetType() );
	boost::shared_ptr<IndexBuffer> indexBuffer = geometryPtr->GetIndexBuffer();
	boost::shared_ptr<VertexBuffer> vertexBuffer = geometryPtr->GetVertexBuffer();

	// bind the buffers
	Spiral::Bind bind1( indexBuffer );
	Spiral::Bind bind2( vertexBuffer );

	if( indexBuffer )
	{
		glDrawElements( primitiveType, static_cast<GLsizei>( indexBuffer->GetIndexCount() ), indexSize, indexBufferPtr );
	}else
	{
		glDrawArrays( primitiveType, 0, static_cast<GLsizei>( vertexBuffer->GetVertexCount() ) );
	}

}