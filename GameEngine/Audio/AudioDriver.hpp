#ifndef AUDIO_DRIVER_HPP
#define AUDIO_DRIVER_HPP

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include "../Core/DeviceDriver.hpp"
#include "../Math/Math.hpp"

namespace Spiral
{

namespace Audio
{
	class AudioObject;
	class AudioStreamObject;

	struct AudioInfo;

	class AudioDriver : public DeviceDriver
	{
	public:

		/*!
		   @function  CreateAudioObject
		   @brief     creates a static audio object
		   @return    bool
		   @param     boost::shared_ptr<AudioObject> & audio
		   @param     const AudioInfo & audioInfo
		*/
		bool CreateAudioObject( boost::shared_ptr<AudioObject>& audio, const AudioInfo& audioInfo, boost::uint32_t bufferSize,
			                    const boost::int8_t* data )
		{
			return DoCreateAudioObject( audio, audioInfo, bufferSize, data );
		}

		/*!
		   @function  CreateAudioStreamObject
		   @brief     creates a streamable audio object
		   @return    bool
		   @param     boost::shared_ptr<AudioStreamObject> & audioStream
		   @param     const AudioInfo & audioInfo
		   @param     boost::uint32_t audioBufferCount
		*/
		bool CreateAudioStreamObject( boost::shared_ptr<AudioStreamObject>& audioStream,
			                          const AudioInfo& audioInfo, boost::uint32_t audioBufferCount )
		{
			return DoCreateAudioStreamObject( audioStream, audioInfo, audioBufferCount );
		}

		void UpdateStreamAudioList()
		{
			DoUpdateStreamAudioList();
		}

	protected:
		AudioDriver(){}

	private:
		virtual void DoUpdateStreamAudioList() = 0;;

		virtual bool DoCreateAudioObject( boost::shared_ptr<AudioObject>& audio, const AudioInfo& audioInfo, boost::uint32_t bufferSize,
			                      const boost::int8_t* data ) = 0;
		virtual bool DoCreateAudioStreamObject( boost::shared_ptr<AudioStreamObject>& audioStream,
			                            const AudioInfo& audioInfo, boost::uint32_t audioBufferCount ) = 0;
	};

} //Audio
}
#endif