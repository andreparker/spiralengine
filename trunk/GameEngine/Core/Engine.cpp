#include "Engine.hpp"
#include "CoreObject.hpp"
#include "GameObjectHandler.hpp"
#include "../Gfx/GfxDriver.hpp"
#include "GameStateMachine.hpp"
#include "GameState.hpp"

using namespace Spiral;
using namespace boost;

Engine::Engine():
m_stateMachine(),
m_gameObjectList(),
m_gfxDriver()
{
}

Engine::~Engine()
{
}

bool Engine::Initialize( shared_ptr< GfxDriver >& gfxDriver, any& data ) 
{
	if( gfxDriver )
	{
		bool result = gfxDriver->Initialize( data );
		if( true == result )
		{
			m_gfxDriver = gfxDriver;
		}
	}

	return bool( m_gfxDriver );
}

void Engine::UnInitialize()
{

}

void Engine::AddGameObject( shared_ptr< CoreObject >& object )
{
	if( object )
	{
		m_gameObjectList->Add( object );
	}
}

void Engine::ClearObjectList()
{
	m_gameObjectList->Clear();
}

void Engine::Tick( real ticks )
{
	m_stateMachine->Tick( ticks );
	m_gameObjectList->Tick( ticks );

	m_gfxDriver->Present();
}

boost::shared_ptr< Engine > Engine::Create()
{
	static boost::shared_ptr< Engine > engine( new Engine );
	return engine;
}

void Engine::AddGameState( shared_ptr< GameState >& state, function< void( GameStateMachine* ) > initFunc )
{
	if( state )
	{
		m_stateMachine->AddState( state, initFunc );
	}
}

void Engine::AddVisualGameState( shared_ptr< VisualGameState >& state, function< void( GameStateMachine* ) > initFunc )
{
	if( state )
	{
		m_stateMachine->AddState( state, initFunc );
	}
}

void Engine::InitializeStateMachine()
{
	m_stateMachine->InitializeStates();
}


void Engine::SetCurrentGameState( shared_ptr< GameState >& state )
{
	if( state )
	{
		m_stateMachine->SetCurrentState( state );
	}
}