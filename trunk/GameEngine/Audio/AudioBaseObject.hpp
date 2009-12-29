#ifndef AUDIO_BASE_OBJECT_HPP
#define AUDIO_BASE_OBJECT_HPP

#include <boost/noncopyable.hpp>

#include "../Core/Cloneable.hpp"
#include "../Math/Math.hpp"

namespace Spiral
{

namespace Audio
{
	class AudioBaseObject : public Cloneable, boost::noncopyable
	{
	public:

		void Play( bool looping = false )
		{
			DoPlay( looping );
		}

		void Stop()
		{
			DoStop();
		}

		void Pause()
		{
			DoPause();
		}

		void SetPosition( const Math::SpVector3r& position )
		{
			DoSetPosition( position );
		}

		const Math::SpVector3r GetPosition()const
		{
			return DoGetPosition();
		}

		void SetVolume( SpReal vol )
		{
			vol = vol < 0.0f ? 0.0f : ( vol > 1.0f ? 1.0f : vol );
			DoSetVolume( vol );
		}

	protected:
		AudioBaseObject(){}

	private:
		virtual void DoPlay( bool looping ) = 0;
		virtual void DoStop() = 0;
		virtual void DoPause() = 0;
		virtual void DoSetVolume( SpReal vol ) = 0;

		virtual void DoSetPosition( const Math::SpVector3r& position ) = 0;
		virtual const Math::SpVector3r DoGetPosition()const = 0;


	};
}
}
#endif