/*!
*/
#ifndef RES_LOCK_IMPL_HPP
#define RES_LOCK_IMPL_HPP

#include <boost/cstdint.hpp>
#include "../Core/Sp_DataTypes.hpp"
#include "../Core/Rect.hpp"

namespace Spiral
{

namespace Impl
{
	struct ResLockInfo
	{
		boost::int32_t size;   ///< size currently locked in bytes
		boost::int8_t* data;  ///< pointer to resource data
	};

	struct ResLockRtInfo
	{
		boost::int32_t rowBytes; ///< bytes per row
		Rect< boost::int32_t > rect;
		boost::int8_t* data;
	};
}

typedef Impl::ResLockInfo ResLockInfo_t;
typedef Impl::ResLockRtInfo ResLockRtInfo_t;

}
#endif