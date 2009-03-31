#include "app.hpp"
#include "types.hpp"

using namespace gameEngine;
using namespace gameEngine::frameWork;

CLASS_RTTI_IMPL( coreApp, gameEngine::object )
{
	DEFINE_ATTR( "screen_size", sizeof(Size2), VAR_OFFSET( m_size ) );
}

coreApp::coreApp():m_appName(0)
{
}

coreApp::coreApp( const int8* name )
{
	setAppName( name );
}

coreApp::~coreApp()
{
	delete [] m_appName;
}

void coreApp::setAppName( const int8* name )
{
	if( name )
	{
		m_appName = new int8[ strlen( name ) + 1 ];
		if( m_appName )
		{
			strcpy( m_appName, name );
		}
	}
}

bool coreApp::initialize()
{ 
	return Super::initialize(); 
}

void coreApp::updateApp( real tick )
{
	Super::update( tick );
	Super::updateChildren( tick );
}

void coreApp::unInitialize()
{
	Super::unInitialize();
}