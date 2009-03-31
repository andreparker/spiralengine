#include "AppFrameWork.hpp"

CLASS_RTTI_IMPL( AppFrameWork, gameEngine::frameWork::coreApp )
{
}

// instance of our app
IMPLEMENT_APP_INSTANCE( AppFrameWork, "AppFrameWork" );
using namespace gameEngine;

AppFrameWork::AppFrameWork( const gameEngine::int8* name ):
frameWork::coreApp( name )
{
	m_size.x = 800;
	m_size.y = 600;
}

AppFrameWork::AppFrameWork()
{
}

AppFrameWork::~AppFrameWork()
{
}

// always call out supers function
bool AppFrameWork::initialize()
{
	return Super::initialize();
}

void AppFrameWork::updateApp( real tick )
{
	Super::updateApp( tick );
}

void AppFrameWork::unInitialize()
{
	Super::unInitialize();
}