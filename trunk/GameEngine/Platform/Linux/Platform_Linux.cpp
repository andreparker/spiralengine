#include "Platform_Linux.hpp"
#include "SDL/Sdl_util.hpp"

using namespace Loki;
using namespace Sdl_Util;
using namespace Spiral;

const Uint32 kSdl_InitFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK;

int main( int argc, char *argv[] )
{
     try
        {
            if( SDL_Succeded( SDL_Init( kSdl_InitFlags ) ) )
            {
                ScopeGuard quitGaurd = MakeGuard( SDL_Quit );
            }
            else
            {
                THROW_GENERAL_EXCEPTION( std::string( "Error - Cannot initialize SDL! :" ) + SDL_GetError() );
            }

        }
        catch( std::exception& e )
        {
        }

}
