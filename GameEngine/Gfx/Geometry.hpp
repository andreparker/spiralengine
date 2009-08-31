/*!
*/
#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include "VertexFormatImplFwd.hpp"
#include "IndexFormatImplFwd.hpp"

namespace Spiral
{

	class VertexBuffer;
	class IndexBuffer;

	class Geometry
	{
	public:
		virtual ~Geometry();

		/*!
		   @function  GetType
		   @brief     return the type of geometry, TRIANGLES,TRIANGLE STRIPS, LINES, etc
		   @return    boost::int32_t
		*/
		boost::int32_t GetType()const
		{
			return m_type;
		}

		/*!
		   @function  CreateHWVertexBuffer
		   @brief     
		   @return    bool
		   @param     const VertexFormat & format
		   @param     boost::int32_t elementSize
		   @param     boost::int32_t vertexCount
		   @param     bool bManaged
		*/
		bool CreateHWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged );
		
		/*!
		   @function  CreateSWVertexBuffer
		   @brief     
		   @return    bool
		   @param     const VertexFormat & format
		   @param     boost::int32_t elementSize
		   @param     boost::int32_t vertexCount
		   @param     bool bManaged
		*/
		bool CreateSWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged );
		
		/*!
		   @function  CreateSWIndexBuffer
		   @brief     
		   @return    bool
		   @param     const IndexFormat & format
		   @param     boost::int32_t indexCount
		   @param     bool bManaged
		*/
		bool CreateSWIndexBuffer( const IndexFormat& format, boost::int32_t indexCount, bool bManaged );
		
		/*!
		   @function  CreateHWIndexBuffer
		   @brief     
		   @return    bool
		   @param     const IndexFormat & format
		   @param     boost::int32_t indexCount
		   @param     bool bManaged
		*/
		bool CreateHWIndexBuffer( const IndexFormat& format, boost::int32_t indexCount, bool bManaged );

		void ReleaseVertexBuffer();
		void ReleaseIndexBuffer();
		void ReleaseBuffers();

		void Bind();
		void UnBind();

		boost::shared_ptr< VertexBuffer >& GetVertexBuffer()
		{
			return m_vertexBuffer;
		}

		boost::shared_ptr< IndexBuffer >& GetIndexBuffer()
		{
			return m_indexBuffer;
		}
	private:
		boost::int32_t m_type;
		boost::shared_ptr< VertexBuffer > m_vertexBuffer;
		boost::shared_ptr< IndexBuffer > m_indexBuffer;
	protected:
		Geometry();

		void SetVertexBuffer( VertexBuffer* vertexBuffer )
		{
			m_vertexBuffer.reset( vertexBuffer );
		}

		void SetIndexBuffer( IndexBuffer* indexBuffer )
		{
			m_indexBuffer.reset( indexBuffer );
		}

		void SetType( boost::int32_t type )
		{
			m_type = type;
		}
	private:

		virtual bool DoCreateHWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged ) = 0;
		virtual bool DoCreateSWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged ) = 0;
		virtual bool DoCreateSWIndexBuffer( const IndexFormat& format, boost::int32_t indexCount, bool bManaged ) = 0;
		virtual bool DoCreateHWIndexBuffer( const IndexFormat& format, boost::int32_t indexCount, bool bManaged ) = 0;
	};
}

#endif