/*!
*/
#ifndef OGL_GEOMETRY_HPP
#define OGL_GEOMETRY_HPP

#include "../Geometry.hpp"
#include "../GeometryTypeFwd.hpp"

namespace Spiral
{
	class OglGeometry : public Geometry
	{
	public:
		OglGeometry( const GeometryType& type );

	private:
		virtual bool DoCreateHWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged );
		virtual bool DoCreateSWVertexBuffer( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged );
		virtual bool DoCreateSWIndexBuffer( const IndexFormat& format, boost::int32_t indexCount, bool bManaged );
		virtual bool DoCreateHWIndexBuffer( const IndexFormat& format, boost::int32_t indexCount, bool bManaged );
	};
}
#endif