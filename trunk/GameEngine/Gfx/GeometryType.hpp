/*!
*/
#ifndef GEOMETRY_TYPE_HPP
#define GEOMETRY_TYPE_HPP

#include <boost/cstdint.hpp>

namespace Spiral
{

namespace Impl
{
	struct GeometryType
	{
		GeometryType():type(INVALID)
		{}

		enum
		{
			INVALID = -1,
			GT_TRIANGLES = 0x0004,
			GT_TRIANGLE_STRIP = 0x0005,
			GT_QUADS = 0x0007,
			GT_POINTS = 0x0000
		};

		boost::int32_t GetType()const
		{
			return type;
		}

		static GeometryType Create_TriangleType()
		{
			return GeometryType( GT_TRIANGLES );
		}

		static GeometryType Create_TriangleStripType()
		{
			return GeometryType( GT_TRIANGLE_STRIP );
		}

		static GeometryType Create_PointType()
		{
			return GeometryType( GT_POINTS );
		}

	private:
		GeometryType( boost::int32_t type_ ):
		type( type_ )
		{
		}
		boost::int32_t type;
	};
}//Impl

typedef Impl::GeometryType GeometryType;

}
#endif