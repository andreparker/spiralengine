#include "GameState.hpp"
#include "StateEventImpl.hpp"

using namespace Spiral;
using namespace boost;

GameState::GameState(boost::int32_t id):
m_id( id ),
m_transList()
{

}

GameState::~GameState()
{
}

void GameState::Enter( Engine* engine )
{
	DoEnter( engine );
}

void GameState::Execute( SpReal tick, Engine* engine )
{
	DoExecute( tick,engine );
}

boost::shared_ptr< GameState > GameState::Transition( const StateEvent& event, Engine* engine )
{
	DoTransition( event, engine );
	return m_transList[ event.eventType ];
}

bool GameState::IsTransition( const StateEvent& event )const
{
	return DoIsTransition( event );
}


void GameState::SetTransitionState( shared_ptr< GameState > gameState, int32_t eventID )
{
	if( m_transList.find( eventID ) == m_transList.end() )
	{
		m_transList[ eventID ] = gameState;
	}
}