#ifndef GFX_DEVICE_CAPABILITY_HPP
#define GFX_DEVICE_CAPABILITY_HPP

#include <boost/cstdint.hpp>

namespace Spiral
{

	struct GfxDeviceCaps
	{
		bool nonPowerOf2Texture;
		boost::uint32_t maxTextureWidth;
		boost::uint32_t maxTextureHeight;
	};
}

#endif