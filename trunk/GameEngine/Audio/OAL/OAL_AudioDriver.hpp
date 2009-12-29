#ifndef OAL_AUDIO_DRIVER_HPP
#define OAL_AUDIO_DRIVER_HPP

#include "../AudioDriver.hpp"

#include <al.h>
#include <alc.h>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <map>
#include <list>

namespace Spiral
{

namespace Audio
{
	struct AudioInfo;
	class OalAudioStreamObject;
	class OalAudioDriver : public AudioDriver
	{
	public:
		OalAudioDriver();
		virtual ~OalAudioDriver();

		const boost::int8_t* GetBufferData( ALuint bufferHandle )const;

		ALenum GetBufferFormatFromInfo( const AudioInfo& info )const;

		void AddStreamObject( OalAudioStreamObject* streamObject );
		void RemoveStreamObject( OalAudioStreamObject* streamObject );

		
	private:
		
		virtual void DoUpdateStreamAudioList();
		bool DoCreateAudioObject( boost::shared_ptr<AudioObject>& audio, const AudioInfo& audioInfo, boost::uint32_t bufferSize, const boost::int8_t* data );
		bool DoCreateAudioStreamObject( boost::shared_ptr<AudioStreamObject>& audioStream,
			                            const AudioInfo& audioInfo, boost::uint32_t audioBufferCount );

		/*!
			@function    DoInitialize
			@brief       initializes data
			@return      bool
			@param       const boost::any & data
		*/
		bool DoInitialize( const boost::any& data );

		/*!
			@function    UnInitialize
			@brief
			@return      bool
		*/
		bool DoUnInitialize();
	private:

		void FillDeviceInfo();
	private:	
		typedef std::list< OalAudioStreamObject* >::iterator Stream_Object_Itr;
		typedef std::map<ALuint,boost::shared_array<boost::int8_t>>::const_iterator Const_Buffer_Itr;
		
		boost::shared_ptr< ALCdevice >  m_audioDevice;
		boost::shared_ptr< ALCcontext > m_context;
		std::map<ALuint,boost::shared_array<boost::int8_t>> m_bufferData;
		std::list< OalAudioStreamObject* > m_streamObjects;
	};
}
}
#endif