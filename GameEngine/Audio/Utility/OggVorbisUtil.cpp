#include "OggVorbisUtil.hpp"
#include "vorbis/vorbisfile.h"

#include "../../Core/File.hpp"
#include "../../Core/Log.hpp"
#include "../AudioInfo.hpp"
#include <boost/assert.hpp>

#include <al.h>
#include <string>
#include <memory.h>

using namespace Spiral;
using namespace Spiral::Audio;
using namespace std;

// 2mb
const boost::uint32_t kMaxBufferSize = 1024 * 1024 * 2;

namespace
{
	size_t readOgg( void* data, size_t size, size_t count, void* srcOggFile )
	{
		IFile* file = static_cast<IFile*>( srcOggFile );

		return static_cast<size_t>( file->Read( reinterpret_cast<boost::int8_t*>(data), count * size ) );
	}

	int    closeOgg( void* srcOggFile )
	{
		IFile* file = static_cast<IFile*>( srcOggFile );
		file->Close();
		return 0;
	}

	long   tellOgg( void* srcOggFile )
	{
		return -1;
	}

	std::string TranslateOggError( int error )
	{
		std::string errorStr;
		switch( error )
		{
		case OV_EREAD:
			errorStr = "A read from media returned an error.";
			break;
		case OV_ENOTVORBIS:
			errorStr = "Bitstream does not contain any Vorbis data.";
			break;
		case OV_EVERSION:
			errorStr = "Vorbis version mismatch.";
			break;
		case OV_EBADHEADER:
			errorStr = "Invalid Vorbis bitstream header.";
			break;
		case OV_EFAULT:
			errorStr = "Internal logic fault; indicates a bug or heap/stack corruption.";
			break;
		default:
			errorStr = "Unknown";
			break;
		}

		return errorStr;
	}

	boost::int32_t ReadOggData( OggVorbis_File& oggFile, boost::int32_t sizeBytes, boost::int8_t* data )
	{
		boost::int32_t totalBytesRead = 0, bytesRead = 0;
		int bitStream = 0;

		do
		{
			bytesRead = ov_read( &oggFile, reinterpret_cast<char *>( data + totalBytesRead ), ( sizeBytes - totalBytesRead), 0, 2, 1, &bitStream );
			if( bytesRead != OV_HOLE ) totalBytesRead += bytesRead;
		}while( bytesRead && totalBytesRead < sizeBytes );

		return totalBytesRead;
	}
}

static ov_callbacks customCallBacks =
{
	readOgg,
	NULL,
	closeOgg,
	tellOgg
};

bool AudioUtil::LoadOgg( const boost::shared_ptr<IFile>& oggFile, boost::scoped_array< boost::int8_t >& data, AudioInfo& info )
{
	OggVorbis_File vorbisFile;
	bool isLoaded = false;

	int error = ov_open_callbacks( oggFile.get(), &vorbisFile, NULL, 0, customCallBacks );

	if( 0 == error )
	{
		vorbis_info* vorb_info = ov_info( &vorbisFile, -1 );  // get the bit stream info

		if( vorb_info )
		{
			info.numBitsPerSample = 16;
			info.numChannels = vorb_info->channels;
			info.rate = vorb_info->rate;

			// allocate a tmp buffer
			boost::int8_t* tmpBuffer = new boost::int8_t[ kMaxBufferSize ];
			boost::int32_t actualSize = ReadOggData( vorbisFile, kMaxBufferSize, tmpBuffer );

			if( actualSize )
			{
				boost::int8_t* newData = new boost::int8_t[ actualSize ];
				memcpy( newData, tmpBuffer, actualSize );
				data.reset( newData );
				isLoaded = true;
			}

			delete [] tmpBuffer;
		}
	}else
	{
		LOG_I( "^wAudioUtil::LoadOgg: ^r" + TranslateOggError( error ) + "\n" );
	}

	ov_clear( &vorbisFile );

	return isLoaded;
}

bool AudioUtil::OpenStreamOgg( const boost::shared_ptr<IFile>& oggFile, AudioUtil::VorbisStreamHandle& handle )
{
	OggVorbis_File* newFile = new OggVorbis_File;
	int error = ov_open_callbacks( oggFile.get(), newFile, NULL, 0, customCallBacks );

	if( error == 0 )
	{
		handle = newFile;
	}

	return bool( error == 0 );
}

boost::uint32_t AudioUtil::StreamOggData( AudioUtil::VorbisStreamHandle& handle, boost::uint32_t size, boost::int8_t* data, bool looping /*= false*/ )
{
	OggVorbis_File* vorbisFile = reinterpret_cast<OggVorbis_File*>(handle);
	boost::uint32_t readSize = ReadOggData( *vorbisFile, size, data );
	IFile* file = static_cast<IFile*>(vorbisFile->datasource);
	
	if( readSize < size && file->Eof() && looping )
	{
		
		do 
		{
			file->Seek( SK_BEGIN, 0 );
			readSize += ReadOggData( *vorbisFile, (size-readSize), (data+readSize) );
		} while ( readSize < size );
		
	}

	return readSize;
}

void AudioUtil::CloseOggStream( AudioUtil::VorbisStreamHandle& handle )
{
	OggVorbis_File* vorbisFile = reinterpret_cast<OggVorbis_File*>(handle);
	ov_clear( vorbisFile );
	delete vorbisFile;

	handle = NULL;
}

void AudioUtil::GetAudioInfoOgg( VorbisStreamHandle& handle, Audio::AudioInfo& info )
{
	OggVorbis_File* vorbisFile = reinterpret_cast<OggVorbis_File*>(handle);
	vorbis_info* vorbisInfo = ov_info(vorbisFile,-1);

	info.numBitsPerSample = 16;
	info.numChannels = vorbisInfo->channels;
	info.rate = vorbisInfo->rate;
}

string GetALErrorString(ALenum err)
{
	switch(err)
	{
	case AL_NO_ERROR:
		return string("AL_NO_ERROR");
		break;

	case AL_INVALID_NAME:
		return string("AL_INVALID_NAME");
		break;

	case AL_INVALID_ENUM:
		return string("AL_INVALID_ENUM");
		break;

	case AL_INVALID_VALUE:
		return string("AL_INVALID_VALUE");
		break;

	case AL_INVALID_OPERATION:
		return string("AL_INVALID_OPERATION");
		break;

	case AL_OUT_OF_MEMORY:
		return string("AL_OUT_OF_MEMORY");
		break;
	};

	return string("UnKnown Error");
}

void AudioUtil::ReportALErrors()
{
	ALenum error = alGetError();
	if( error != AL_NO_ERROR )
	{
		string errorStr = GetALErrorString( error );
		LOG_I( "^rAudioUtil::ReportALErrors: OpenAL - " + errorStr + "\n" );
		BOOST_ASSERT( 0 );
	}
}
