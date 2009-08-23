/*!
*/
#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <boost/scoped_ptr.hpp>
#include <boost/cstdint.hpp>

namespace Spiral
{

	class VertexBuffer;
	class IndexBuffer;

	class Geometry
	{
	public:

		boost::int32_t GetType()const
		{
			return m_type;
		}

	private:
		boost::int32_t m_type;
		boost::scoped_ptr< VertexBuffer > m_vertexBuffer;
		boost::scoped_ptr< IndexBuffer > m_indexBuffer;
	private:

		void SetVertexBuffer( VertexBuffer* vertexBuffer )
		{
			m_vertexBuffer.reset( vertexBuffer );
		}

		void SetIndexBuffer( IndexBuffer* indexBuffer )
		{
			m_indexBuffer.reset( indexBuffer );
		}

	};
}

#endif