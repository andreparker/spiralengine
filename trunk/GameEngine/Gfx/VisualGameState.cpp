#include "VisualGameState.hpp"
#include "../Core/GameState.hpp"

using namespace Spiral;
using namespace boost;

VisualGameState::VisualGameState( int32_t id ):
m_gameState(),
m_id( id )
{

}

void VisualGameState::Enter( Engine* engine )
{
	DoEnter( engine );
}

void VisualGameState::Execute( SpReal tick, Engine* engine )
{
	DoExecute( tick, engine );
}

void VisualGameState::Transition( Engine* engine )
{
	DoTransition( engine );
}

void VisualGameState::Attach( shared_ptr< GameState >& state )
{
	if( state )
	{
		m_gameState = state;
	}
}

VisualGameState::~VisualGameState()
{

}