/*!
*/
#ifndef GAME_STATE_MACHINE_HPP
#define GAME_STATE_MACHINE_HPP

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <queue>
#include <list>
#include <map>

#include "StateEventImpl.hpp"
#include "Sp_DataTypes.hpp"

namespace Spiral
{
	class GameState;
	class VisualGameState;
	class GameStateMachine;
	class Engine;
	
	typedef boost::function< void( GameStateMachine* ) > StateInitFunc;

	class GameStateMachine : private boost::noncopyable
	{
	public:
		GameStateMachine();

		/*!
		   @function  ProcessEvent
		   @brief     lets the current state process this event
		   @return    void
		   @param     const StateEvent & event
		*/
		void ProcessEvent( const StateEvent& event );

		/*!
		   @function  SetCurrentState
		   @brief     sets the current state to run
		   @return    void
		   @param     boost::shared_ptr< GameState > & state
		   @param     Engine* engine
		*/
		void SetCurrentState( boost::shared_ptr< GameState >& state, Engine* engine );

		/*!
		   @function  InitializeStates
		   @brief     Initializes all states that have been added e.g calls the function call backs
		   @return    void
		*/
		void InitializeStates();

		/*!
		   @function  ClearStates
		   @brief     removes all states from the state machine and callbacks
		   @return    void
		*/
		void ClearStates();

		void UnInitializeStates();
		/*!
		   @function  AddState
		   @brief     adds a unique state to the state machine
		   @return    void
		   @param     boost::shared_ptr< GameState > & state - state to add
		   @param     boost::function< void(GameStateMachine *)> initFunc - function to initialize state transitions
		*/
		void AddState( boost::shared_ptr< GameState >& state, StateInitFunc initFunc );
		void AddState( boost::shared_ptr< VisualGameState >& state, StateInitFunc initFunc );

		/*!
		   @function  Tick
		   @brief     updates the state machine
		   @return    void
		   @param     SpReal tick - frac seconds
		*/
		void Tick( SpReal tick, Engine* engine );

		/*!
		   @function  GetState
		   @brief     returns a state by a id
		   @return    boost::shared_ptr< GameState >
		   @param     boost::int32_t stateId
		*/
		boost::shared_ptr< GameState > GetState( boost::int32_t stateId )const;

	private:
		void ProcessEvents( Engine* engine );
	private:
		typedef std::list< boost::shared_ptr< GameState > >::const_iterator const_state_itr;
		std::list< boost::shared_ptr< GameState > >                         m_gameStateList;
		std::map< boost::int32_t, boost::shared_ptr< VisualGameState > >    m_visualGameStateList;
		std::list< boost::function< void( GameStateMachine* ) > >           m_stateFuncInitList;
		boost::shared_ptr< GameState >                                      m_currentGameState;
		boost::shared_ptr< VisualGameState >                                m_currentVisualState;
		std::queue< StateEvent >                                            m_eventQueue;

	};
}

#endif