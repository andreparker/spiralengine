#include "OAL_AudioObject.hpp"
#include "OAL_AudioDriver.hpp"
#include "../../Math/Math.hpp"
#include "../Utility/OggVorbisUtil.hpp"
#include <alc.h>

using namespace Spiral;
using namespace Spiral::Audio;

OalAudioObject::OalAudioObject( ALuint sourceHandle, ALuint bufferHandle, ALuint dataHandle, OalAudioDriver* driver ):
m_sourceHandle( sourceHandle ),m_bufferHandle( bufferHandle ),m_dataHandle(dataHandle),m_audioDriver( driver )
{

}

void OalAudioObject::DoPlay( bool looping )
{
	if( IsValidBuffer() )
	{
		ALint loop = AL_FALSE;
		ALint state;

		if( looping == true )
		{
			loop = AL_TRUE;
		}

		alGetSourcei( m_sourceHandle, AL_SOURCE_STATE, &state );
		alSourcei( m_sourceHandle, AL_LOOPING, loop );

		if( state != AL_PLAYING )
		{
			alSourcePlay( m_sourceHandle );
		}
	}
}

void OalAudioObject::DoStop()
{
	if( IsValidBuffer() )
	{
		alSourceStop( m_sourceHandle );
	}
}

bool OalAudioObject::IsValidBuffer() const
{
	return bool( m_bufferHandle != 0 && alIsBuffer( m_bufferHandle ) == AL_TRUE );
}

OalAudioObject::~OalAudioObject()
{
	Stop();
	alDeleteBuffers( 1, &m_bufferHandle );
	alDeleteSources( 1, &m_sourceHandle );
}

void OalAudioObject::DoPause()
{
	if( IsValidBuffer() )
	{
		alSourcePause( m_sourceHandle );
	}
}

void OalAudioObject::DoSetPosition( const Math::Vector3f& position )
{
	alSource3f( m_sourceHandle, AL_POSITION, position[0], position[1], position[2] );
}

const Math::Vector3f OalAudioObject::DoGetPosition() const
{
	ALfloat position[3];
	alGetSourcefv( m_sourceHandle, AL_POSITION, position );

	return Math::Vector3f( position[0], position[1], position[2] );
}

Cloneable* OalAudioObject::DoClone() const
{
	ALuint srcHandle,bufferHandle = 0;

	alGenSources( 1, &srcHandle );
	if( IsValidBuffer() )
	{
		alGenBuffers( 1, &bufferHandle );
		CloneBufferToHandle( bufferHandle );
	}

	OalAudioObject* cloneObject = new OalAudioObject( srcHandle, bufferHandle, m_dataHandle, m_audioDriver );

	return cloneObject;

}

void OalAudioObject::CloneBufferToHandle( ALuint toHandle )const
{
	ALint channels, size, bits, freq;
	ALenum format;

	alGetBufferi( m_bufferHandle, AL_CHANNELS, &channels );
	alGetBufferi( m_bufferHandle, AL_SIZE, &size );
	alGetBufferi( m_bufferHandle, AL_BITS, &bits );
	alGetBufferi( m_bufferHandle, AL_FREQUENCY, &freq );

	if( bits == 16 )
	{
		format = channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	}else if( bits == 8 )
	{
		format = channels > 1 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	}

	const boost::int8_t* srcData = m_audioDriver->GetBufferData( m_dataHandle );

	if( srcData != NULL )
	{
		alBufferData( toHandle, format, srcData, size, freq );
	}
}

void OalAudioObject::DoSetVolume( SpReal vol )
{
	alSourcef( m_sourceHandle, AL_GAIN, vol );
	AudioUtil::ReportALErrors();
}