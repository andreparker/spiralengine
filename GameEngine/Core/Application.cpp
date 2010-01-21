#include "Application.hpp"
#include "Engine.hpp"
#include "GeneralException.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;

bool Application::Init( int32_t argc, list< boost::shared_array< char > >& argList, boost::shared_ptr< Engine >& engine )
{
	if( !engine )
	{
		THROW_GENERAL_EXCEPTION( "Invalid engine pointer!" );
	}

	return DoInit( argc, argList, engine );
}

bool Application::Run( SpReal ticks, boost::shared_ptr< Engine >& engine )
{
	return DoRun( ticks, engine );
}

bool Application::UnInit()
{
	return DoUnInit();
}