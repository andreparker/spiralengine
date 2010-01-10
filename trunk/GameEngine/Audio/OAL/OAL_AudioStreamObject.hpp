#ifndef OAL_AUDIO_STREAM_OBJECT_HPP
#define OAL_AUDIO_STREAM_OBJECT_HPP

#include <al.h>
#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include "../AudioInfo.hpp"
#include "../AudioStreamObject.hpp"
#include "../Utility/OggVorbisUtil.hpp"

namespace Spiral
{

namespace Audio
{
	const boost::uint32_t kMaxStreamBufferSize = 1024 * 512;

	class OalAudioDriver;
	class OalAudioStreamObject : public AudioStreamObject
	{
	public:
		OalAudioStreamObject( ALuint srcHandle, const ALuint* bufferHandles, boost::uint32_t bufferCount, const AudioInfo& info, OalAudioDriver* driver );
		virtual ~OalAudioStreamObject();
		
		void Stream();
	private:
		virtual void DoPreLoadData();

		void FreeHandles( ALuint* handles )const;

		virtual void DoPlay( bool looping );
		virtual void DoStop();
		virtual void DoPause();
		virtual void DoSetVolume( SpReal vol );

		virtual void DoSetPosition( const Math::Vector3f& position );
		virtual const Math::Vector3f DoGetPosition()const;

		virtual Cloneable* DoClone()const;

		void StreamDataToBuffer( ALuint bufferHandle, bool isLooping );
	private:
		ALuint m_sourceHandle;
		boost::shared_array< ALuint > m_bufferHandles;
		boost::scoped_array< boost::int8_t > m_dataBuffer;
		AudioUtil::VorbisStreamHandle m_streamHandle;
		AudioInfo m_info;
		OalAudioDriver* m_audioDriver;
		boost::uint32_t m_bufferCount;
		bool m_isPreLoaded;
		bool m_isLooping;
		boost::uint32_t m_unUsedBufferIndex;
	};
}
}
#endif