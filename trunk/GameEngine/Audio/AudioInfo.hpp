#ifndef AUDIO_INFO_HPP
#define AUDIO_INFO_HPP

#include <boost/cstdint.hpp>

namespace Spiral
{

namespace Audio
{

	struct AudioInfo
	{
		boost::uint32_t numChannels;       ///< 1,2
		boost::uint32_t numBitsPerSample;  ///< 8,16
		boost::uint32_t rate;              ///< bit rate

	};
}
}
#endif