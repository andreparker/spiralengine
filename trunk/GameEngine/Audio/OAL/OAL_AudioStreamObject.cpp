#include "OAL_AudioStreamObject.hpp"
#include "OAL_AudioDriver.hpp"
#include "../../Core/File.hpp"

#include <boost/bind.hpp>
#include <memory.h>

using namespace Spiral;
using namespace Spiral::Audio;

const boost::int32_t kMaxLoadBuffers = 2;

void OalAudioStreamObject::Stream()
{
	ALint bufferQueseStatus,looping;
	ALuint currentBuffer;
	ALenum format;
	ALsizei size;

	alGetSourcei( m_sourceHandle, AL_BUFFERS_PROCESSED, &bufferQueseStatus );
	AudioUtil::ReportALErrors();

	bool isLooping = m_isLooping;
	format = m_audioDriver->GetBufferFormatFromInfo( m_info );

	if( m_unUsedBufferIndex < m_bufferCount )
	{
		for( int i = 0; i < 2 && m_unUsedBufferIndex < m_bufferCount; ++i, ++m_unUsedBufferIndex )
		{
			StreamDataToBuffer( m_bufferHandles[m_unUsedBufferIndex], false );
			alSourceQueueBuffers( m_sourceHandle, 1, &m_bufferHandles[m_unUsedBufferIndex] );
		}
	}else
	{
		if( bufferQueseStatus > kMaxLoadBuffers )
			bufferQueseStatus = kMaxLoadBuffers;

		while( bufferQueseStatus-- > 0 )
		{
			alSourceUnqueueBuffers( m_sourceHandle, 1, &currentBuffer );
			AudioUtil::ReportALErrors();
			size = static_cast<ALsizei>( AudioUtil::StreamOggData( m_streamHandle, kMaxStreamBufferSize, m_dataBuffer.get(), isLooping ) );

			alBufferData( currentBuffer, format, m_dataBuffer.get(), size, m_info.rate );
			AudioUtil::ReportALErrors();

			alSourceQueueBuffers( m_sourceHandle, 1, &currentBuffer );
			AudioUtil::ReportALErrors();
		}
	}
	
}

OalAudioStreamObject::OalAudioStreamObject( ALuint srcHandle, const ALuint* bufferHandles, boost::uint32_t bufferCount, const AudioInfo& info, OalAudioDriver* driver ):
m_sourceHandle(srcHandle),m_bufferHandles(),m_dataBuffer(),m_streamHandle(NULL),m_info(info),m_audioDriver(driver),m_bufferCount(bufferCount),m_isPreLoaded(false),m_unUsedBufferIndex(0)
{
	m_bufferHandles.reset( new ALuint[bufferCount], boost::bind( &OalAudioStreamObject::FreeHandles, this, _1 ) );
	m_dataBuffer.reset( new boost::int8_t[ kMaxStreamBufferSize ] );
 
	memcpy( m_bufferHandles.get(), bufferHandles, bufferCount * sizeof(ALuint) );
} 

void OalAudioStreamObject::DoPreLoadData()
{
	if( false == m_isPreLoaded && GetStreamFile())
	{
		AudioUtil::OpenStreamOgg( GetStreamFile(), m_streamHandle );
		AudioUtil::GetAudioInfoOgg( m_streamHandle, m_info );
		StreamDataToBuffer( m_bufferHandles[0], false );
		StreamDataToBuffer( m_bufferHandles[1], false );
		alSourceQueueBuffers( m_sourceHandle, 2, m_bufferHandles.get() );
		AudioUtil::ReportALErrors();
		m_unUsedBufferIndex = 2;  // set to next buffer to fill with data
		m_isPreLoaded = true;
	}
}

void OalAudioStreamObject::FreeHandles( ALuint* handles ) const
{
	alDeleteBuffers( m_bufferCount, handles );
	delete [] handles;
}

OalAudioStreamObject::~OalAudioStreamObject()
{
	Stop();
	AudioUtil::CloseOggStream( m_streamHandle );
}

void OalAudioStreamObject::DoPlay( bool looping )
{
	if( GetStreamFile() )
	{
		if( GetStreamFile()->Eof() )
		{
			// reset file position
			GetStreamFile()->Seek( SK_BEGIN, 0 );
		}

		m_isLooping = looping;
		alSourcePlay( m_sourceHandle );
		AudioUtil::ReportALErrors();
		m_audioDriver->AddStreamObject( this );
	}
}

void OalAudioStreamObject::DoStop()
{
	alSourceStop( m_sourceHandle );
	AudioUtil::ReportALErrors();
	m_audioDriver->RemoveStreamObject( this );
}

void OalAudioStreamObject::DoPause()
{ 
	alSourcePause( m_sourceHandle );
	AudioUtil::ReportALErrors();
	m_audioDriver->RemoveStreamObject( this );
}

void OalAudioStreamObject::DoSetPosition( const Math::SpVector3r& position )
{
	alSource3f( m_sourceHandle, AL_POSITION, position[0], position[1], position[2] );
}

const Math::SpVector3r OalAudioStreamObject::DoGetPosition() const
{
	ALfloat position[3];
	alGetSourcefv( m_sourceHandle, AL_POSITION, position );
	return Math::SpVector3r( position[0],position[1],position[2] );
}

Cloneable* OalAudioStreamObject::DoClone() const
{
	return NULL;
}

void OalAudioStreamObject::StreamDataToBuffer( ALuint bufferHandle, bool isLooping )
{
	ALenum format = m_audioDriver->GetBufferFormatFromInfo( m_info );

	ALsizei size = static_cast<ALsizei>( AudioUtil::StreamOggData( m_streamHandle, kMaxStreamBufferSize, m_dataBuffer.get(), isLooping ) );

	alBufferData( bufferHandle, format, m_dataBuffer.get(), size, m_info.rate );
	AudioUtil::ReportALErrors();
}

void OalAudioStreamObject::DoSetVolume( SpReal vol )
{
	alSourcef( m_sourceHandle, AL_GAIN, vol );
	AudioUtil::ReportALErrors();
}