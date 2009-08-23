/*!
*/
#ifndef VERTEX_FORMAT_IMPL_HPP
#define VERTEX_FORMAT_IMPL_HPP

#include <boost/cstdint.hpp>

namespace Spiral
{

namespace Impl
{
	struct VertexFormat
	{
		VertexFormat():type( VF_INVALID ){}

		enum
		{
			VF_INVALID = -1,
			VF_V3,			///< 3 component vertices xyz
			VF_V3T2			///< xyz t,s texture coords
		};

		boost::int32_t GetType()const
		{
			return type;
		}

		static VertexFormat Create_V3()
		{
			return VertexFormat( VF_V3 );
		}

		static VertexFormat Create_V3T2()
		{
			return VertexFormat( VF_V3T2 );
		}
	private:
		boost::int32_t type;
		VertexFormat( boost::int32_t type_ ):
			type( type_ ){}
	};
}

typedef Impl::VertexFormat VertexFormat;

}

#endif