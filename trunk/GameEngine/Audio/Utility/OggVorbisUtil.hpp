#ifndef OGG_VORBIS_UTIL_HPP
#define OGG_VORBIS_UTIL_HPP

#include <string>
#include <boost/cstdint.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

namespace Spiral
{

	class IFile;
namespace Audio
{
	struct AudioInfo;
}

namespace AudioUtil
{
	typedef void* VorbisStreamHandle;

	bool LoadOgg( const boost::shared_ptr<IFile>& oggFile, boost::scoped_array< boost::int8_t >& data, Audio::AudioInfo& info );
	bool OpenStreamOgg( const boost::shared_ptr<IFile>& oggFile, VorbisStreamHandle& handle );
	void GetAudioInfoOgg( VorbisStreamHandle& handle, Audio::AudioInfo& info );
	boost::uint32_t StreamOggData( VorbisStreamHandle& handle, boost::uint32_t size, boost::int8_t* data, bool looping = false );
	void CloseOggStream( VorbisStreamHandle& handle );
	void ReportALErrors();
}
}
#endif