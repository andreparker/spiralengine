#include "Application.hpp"
#include "Engine.hpp"
#include "GeneralException.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;

bool Application::Init( int32_t argc, list< shared_array< char > >& argList, shared_ptr< Engine >& engine )
{
	if( !engine )
	{
		throw GeneralException( "Invalid engine pointer!" );
	}

	return DoInit( argc, argList, engine );
}

bool Application::Run( real ticks, shared_ptr< Engine >& engine )
{
	return DoRun( ticks, engine );
}

bool Application::UnInit()
{
	return DoUnInit();
}