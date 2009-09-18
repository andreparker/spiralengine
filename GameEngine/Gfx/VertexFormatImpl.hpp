/*!
*/
#ifndef VERTEX_FORMAT_IMPL_HPP
#define VERTEX_FORMAT_IMPL_HPP

#include <boost/cstdint.hpp>
#include <boost/tuple/tuple.hpp>

#include "../Core/Sp_DataTypes.hpp"

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

		enum
		{
			VFS_V3 = 3 * sizeof(SpReal),
			VFS_V3T2 = 5 * sizeof(SpReal)
		};

		typedef boost::tuples::tuple< SpReal, SpReal, SpReal, SpReal, SpReal > V3T2_Storage;

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

		static void SetV3T2Data( SpReal* aryData, const V3T2_Storage& data )
		{
			// vertex x,y,z
			aryData[ 0 ] = boost::tuples::get< 0 >( data );
			aryData[ 1 ] = boost::tuples::get< 1 >( data );
			aryData[ 2 ] = boost::tuples::get< 2 >( data );

			// texure s,t
			aryData[ 3 ] = boost::tuples::get< 3 >( data );
			aryData[ 4 ] = boost::tuples::get< 4 >( data );

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