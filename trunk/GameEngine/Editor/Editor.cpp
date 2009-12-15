#include "Editor.hpp"

using namespace Editor;
using namespace Spiral;

App::App():
m_engine()
{

}

bool App::DoInit( boost::int32_t argc, std::list< boost::shared_array< char > >& argList, boost::shared_ptr< Spiral::Engine >& engine )
{
	bool isInitialize = true;
	m_engine = engine;  // get a weak ptr to the engine

	return isInitialize;
}


bool App::DoRun( Spiral::SpReal ticks, boost::shared_ptr< Spiral::Engine >& engine )
{

	return true;
}

bool App::DoUnInit()
{

	return true;
}