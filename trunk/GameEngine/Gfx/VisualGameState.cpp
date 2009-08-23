#include "VisualGameState.hpp"
#include "../Core/GameState.hpp"

using namespace Spiral;
using namespace boost;

VisualGameState::VisualGameState( int32_t id ):
m_gameState(),
m_id( id )
{

}

void VisualGameState::Enter()
{
	DoEnter();
}

void VisualGameState::Execute( real tick )
{
	DoExecute( tick );
}

void VisualGameState::Transition()
{
	DoTransition();
}

void VisualGameState::Attach( shared_ptr< GameState >& state )
{
	if( state )
	{
		m_gameState = state;
	}
}