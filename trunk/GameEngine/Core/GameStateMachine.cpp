#include <algorithm>
#include <boost/bind.hpp>

#include "GameStateMachine.hpp"
#include "GameState.hpp"
#include "../Gfx/VisualGameState.hpp"
#include "StateEventImpl.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;

GameStateMachine::GameStateMachine():
m_gameStateList(),
m_visualGameStateList(),
m_stateFuncInitList(),
m_currentGameState(),
m_currentVisualState()
{

}

template< class argType, class valType >
inline bool FindVal( argType& col, valType& val )
{
	return bool( find( col.begin(), col.end(), val ) != col.end() );
}


void GameStateMachine::InitializeStates()
{
	struct funcObject
	{
		void operator()( StateInitFunc func, GameStateMachine* data_ )
		{
			func( data_ );
		}

	} funcObj;

	for_each( m_stateFuncInitList.begin(), m_stateFuncInitList.end(), bind<void>( ref(funcObj), _1, this ) );
}

void GameStateMachine::ClearStates()
{
	m_gameStateList.clear();
	m_visualGameStateList.clear();
	m_stateFuncInitList.clear();
}

void GameStateMachine::AddState( shared_ptr<GameState> &state, StateInitFunc& initFunc )
{
	// no duplicates
	m_stateFuncInitList.push_back( initFunc );
	if( FindVal( m_gameStateList, state ) != true )
	{
		m_gameStateList.push_back( state );
	}
		
}

void GameStateMachine::SetCurrentState( boost::shared_ptr< GameState >& state )
{
	m_currentGameState = state;
	m_currentGameState->Enter();
}

shared_ptr< GameState > GameStateMachine::GetState( int32_t stateId )const
{
	shared_ptr< GameState > state;
	const_state_itr itr = find_if( m_gameStateList.begin(), m_gameStateList.end(), bind( &GameState::GetID, _1 ) == stateId );
	if( itr != m_gameStateList.end() )
	{
		state = *itr;
	}

	return state;
}

void GameStateMachine::Tick( real tick )
{
	if( m_currentGameState )
	{
		m_currentGameState->Execute( tick );
	}

	if( m_currentVisualState )
	{
		// TODO: may need to pass a gfxDriver to this
		m_currentVisualState->Execute( tick );
	}
}

void GameStateMachine::UnInitializeStates()
{

}

void GameStateMachine::AddState( boost::shared_ptr< VisualGameState >& state, StateInitFunc& initFunc )
{
	if( m_visualGameStateList.find( state->GetID() ) != m_visualGameStateList.end() )
	{
		m_visualGameStateList[ state->GetID() ] = state;
	}
		m_stateFuncInitList.push_back( initFunc );
}

void GameStateMachine::ProcessEvent( const StateEvent& event )
{
	if( m_currentGameState )
	{
		if( m_currentGameState->IsTransition( event ) == true )
		{
			shared_ptr< GameState > newState = m_currentGameState->Transition( event );
			if( m_currentVisualState )
			{
				m_currentVisualState->Transition();
				m_currentVisualState = m_visualGameStateList[ m_currentGameState->GetID() ];
				m_currentVisualState->Enter();
			}
			newState->Enter();
			m_currentGameState = newState;
		}
	}
}