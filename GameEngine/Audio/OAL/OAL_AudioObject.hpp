#ifndef OAL_AUDIO_OBJECT_HPP
#define OAL_AUDIO_OBJECT_HPP

#include "../AudioObject.hpp"
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <al.h>

namespace Spiral
{

namespace Audio
{
	class OalAudioDriver;
	class OalAudioObject : public AudioObject
	{
	public:
		OalAudioObject( ALuint sourceHandle, ALuint bufferHandle, ALuint dataHandle, OalAudioDriver* driver );
		virtual ~OalAudioObject();

	private:
		virtual void DoPlay( bool looping );
		virtual void DoStop();
		virtual void DoPause();
		virtual void DoSetVolume( SpReal vol );

		virtual void DoSetPosition( const Math::SpVector3r& position );
		virtual const Math::SpVector3r DoGetPosition()const;

		virtual Cloneable* DoClone()const;

		bool IsValidBuffer()const;
		void CloneBufferToHandle( ALuint toHandle )const;

	private:
		ALuint m_sourceHandle;
		ALuint m_bufferHandle;
		ALuint m_dataHandle;
		OalAudioDriver* m_audioDriver;
	};
}
}
#endif