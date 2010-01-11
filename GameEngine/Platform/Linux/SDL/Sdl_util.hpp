#ifndef SDL_UTIL_HPP
#define SDL_UTIL_HPP

#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>

namespace Spiral
{
    class EventPublisher;
}

namespace Sdl_Util
{

    enum
    {
        SDL_ERROR = -1,
        SDL_SUCCESS
    };

    enum
    {
        SDL_EVENT_NONE = 0,
        SDL_EVENT_PENDING
    };

    inline bool SDL_Succeded( int result_ )
    {
        return bool( ( result_ ) == SDL_SUCCESS ? true : false );
    }

    void ProccessEvents( const boost::shared_ptr< Spiral::EventPublisher >& publisher );
}

#endif
