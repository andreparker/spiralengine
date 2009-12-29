#ifndef AUDIO_STREAM_OBJECT_HPP
#define AUDIO_STREAM_OBJECT_HPP

#include "AudioBaseObject.hpp"
#include <boost/shared_ptr.hpp>

namespace Spiral
{
	class IFile;
namespace Audio
{

	class AudioStreamObject : public AudioBaseObject
	{
	public:

		void SetStreamFile( const boost::shared_ptr<IFile>& file )
		{
			m_streamFile = file;
		}

		void Play( bool looping = false )
		{
			PreLoadData();
			AudioBaseObject::Play( looping );
		}

		void PreLoadData()
		{
			DoPreLoadData();
		}

	protected:
		AudioStreamObject():m_streamFile(){}

		const boost::shared_ptr<IFile>& GetStreamFile()const
		{
			return m_streamFile;
		}
	private:
		virtual void DoPreLoadData() = 0;

		boost::shared_ptr<IFile> m_streamFile;

	};
}
}
#endif