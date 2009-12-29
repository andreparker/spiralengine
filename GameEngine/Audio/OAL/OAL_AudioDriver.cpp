#include "OAL_AudioDriver.hpp"
#include "OAL_AudioObject.hpp"
#include "OAL_AudioStreamObject.hpp"
#include "../../Core/Log.hpp"
#include "../AudioInfo.hpp"
#include "../Utility/OggVorbisUtil.hpp"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <string>
#include <memory.h>
#include <algorithm>

using namespace Spiral;
using namespace Spiral::Audio;

const std::string module = "^OalAudioDriver :";
const boost::int32_t kMaxHandleCount = 10;

OalAudioDriver::OalAudioDriver():
m_audioDevice(),m_context(),m_bufferData(),m_streamObjects()
{

}

bool OalAudioDriver::DoCreateAudioObject( boost::shared_ptr<AudioObject>& audio, const AudioInfo& audioInfo , 
										  boost::uint32_t bufferSize, const boost::int8_t* data )
{
	ALuint srcHandle, bufferHandle = 0;
	boost::shared_ptr<AudioObject> audioObject;

	alGenSources( 1, &srcHandle );
	alSourcef( srcHandle, AL_GAIN, 1.0f );  // max volume
	AudioUtil::ReportALErrors();
	
	if( data )
	{
		alGenBuffers( 1, &bufferHandle );
		AudioUtil::ReportALErrors();
		ALenum format = GetBufferFormatFromInfo( audioInfo );
		alBufferData( bufferHandle, format, data, bufferSize, audioInfo.rate );
		AudioUtil::ReportALErrors();
		boost::shared_array< boost::int8_t > newArray( new boost::int8_t[ bufferSize ] );
		memcpy( newArray.get(), data, bufferSize );

		m_bufferData.insert( std::pair<ALuint,boost::shared_array<boost::int8_t>>( bufferHandle, newArray ) );
	}

	audio = boost::make_shared<OalAudioObject>( srcHandle, bufferHandle, bufferHandle, this );

	return bool( alIsSource( srcHandle ) == AL_TRUE );
}

bool OalAudioDriver::DoCreateAudioStreamObject( boost::shared_ptr<AudioStreamObject>& audioStream,
											    const AudioInfo& audioInfo, boost::uint32_t audioBufferCount )
{
	boost::shared_ptr<AudioStreamObject> streamObject;
	ALuint srcHandle, bufferHandles[kMaxHandleCount];

	alGenSources( 1, &srcHandle );
	alSourcef( srcHandle, AL_GAIN, 1.0f );  // max volume
	AudioUtil::ReportALErrors();

	if( audioBufferCount <= kMaxHandleCount )
	{
		alGenBuffers( static_cast<ALsizei>(audioBufferCount), bufferHandles );
		AudioUtil::ReportALErrors();
		streamObject = boost::make_shared<OalAudioStreamObject>( srcHandle, bufferHandles, audioBufferCount, audioInfo, this );
	}

	audioStream = streamObject;

	return bool( alIsSource( srcHandle ) == AL_TRUE );
}

bool OalAudioDriver::DoInitialize( const boost::any& data )
{
	LOG_I( module + "^wInitializing device...\n" );
	bool isInitialized = false;
	ALCdevice* audioDevice = alcOpenDevice( NULL );

	if( audioDevice != NULL )
	{
		m_audioDevice.reset( audioDevice, boost::bind( alcCloseDevice, _1 ) );
		ALCcontext* context = alcCreateContext( audioDevice, NULL );
		if( context )
		{
			ALCboolean result = alcMakeContextCurrent( context );
			if( result == ALC_TRUE )
			{
				m_context.reset( context, boost::bind( alcDestroyContext, _1 ) );
				FillDeviceInfo();
				LOG_I( module + " ^w ----------Driver Info---------- " );
				LOG_I( "^g" + GetName() + GetInfo() + "\n" );
				LOG_I( module + " ^w --------------End-------------- \n" );
				isInitialized = true;
			}

		}
	}

	return isInitialized;
}

bool OalAudioDriver::DoUnInitialize()
{
	m_context.reset();
	m_audioDevice.reset();
	m_bufferData.clear();
	m_streamObjects.clear();
	return true;
}

void OalAudioDriver::FillDeviceInfo()
{
	std::string deviceInfo = "^yAudio Device :^w";
	deviceInfo += alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER );
	SetName( deviceInfo );

	deviceInfo = "\n^yAudio Devices :^w";
	deviceInfo += alcGetString( NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER );
	deviceInfo += std::string("\n^yOut Device: ^w") + alGetString( AL_RENDERER );
	deviceInfo += std::string("\n^yVendor: ^w") + alGetString( AL_VENDOR );
	deviceInfo += std::string("\n^yVersion: ^w") + alGetString( AL_VERSION );
	deviceInfo += "\n^yDevice Extensions: ^g\n";
	deviceInfo += alGetString( AL_EXTENSIONS );

	SetInfo( deviceInfo );
}

const boost::int8_t* OalAudioDriver::GetBufferData( ALuint bufferHandle ) const
{
	boost::int8_t* data = NULL;
	Const_Buffer_Itr itr = m_bufferData.find( bufferHandle );

	if( itr != m_bufferData.end() )
	{
		data = (*itr).second.get();
	}

	return data;
}

ALenum OalAudioDriver::GetBufferFormatFromInfo( const AudioInfo& info ) const
{
	ALenum format = AL_FORMAT_STEREO16;

	if( info.numBitsPerSample == 16 )
	{
		format = info.numChannels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	}else if( info.numBitsPerSample == 8 )
	{
		format = info.numChannels > 1 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	}

	return format;
}

void OalAudioDriver::AddStreamObject( OalAudioStreamObject* streamObject )
{
	if ( std::find( m_streamObjects.begin(), m_streamObjects.end(), streamObject ) == m_streamObjects.end() )
	{
		m_streamObjects.push_back( streamObject );
	}
}

void OalAudioDriver::RemoveStreamObject( OalAudioStreamObject* streamObject )
{
	m_streamObjects.remove( streamObject );
}

void OalAudioDriver::DoUpdateStreamAudioList()
{
	for( Stream_Object_Itr itr = m_streamObjects.begin(); itr != m_streamObjects.end(); ++itr )
	{
		(*itr)->Stream();
	}
}

OalAudioDriver::~OalAudioDriver()
{
}