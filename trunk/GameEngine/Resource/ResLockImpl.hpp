/*!
*/
#ifndef RES_LOCK_IMPL_HPP
#define RES_LOCK_IMPL_HPP

#include "../Core/Sp_DataTypes.hpp"

namespace Spiral
{

namespace Impl
{
	struct ResLockInfo
	{
		boost::int32_t stride; ///< element size bytes
		boost::int32_t size;   ///< size currently locked in bytes
		boost::uint8_t* data;  ///< pointer to resource data
	};

	struct ResLockRtInfo
	{
		boost::int32_t stride;
		boost::int32_t rowBytes; ///< bytes per row
		Rect< boost::int32_t > rect;
		boost::uint8_t* data;
	};
}

typedef Impl::ResLockInfo ResLockInfo_t;
typedef Impl::ResLockRtInfo ResLockRtInfo_t;

}
#endif