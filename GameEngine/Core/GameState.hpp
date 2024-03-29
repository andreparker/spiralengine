/*!
*/
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <map>

#include "Sp_DataTypes.hpp"
#include "StateEventImplFwd.hpp"

namespace Spiral
{

	class Engine;
	class GameState : private boost::noncopyable
	{
	public:
		virtual ~GameState();

		/*!
		   @function  Enter
		   @brief     called on start of the state
		   @return    void
		*/
		void Enter( Engine* engine );


		/*!
		   @function  Execute
		   @brief     states main execute function
		   @return    void
		   @param     SpReal tick - frac seconds
		*/
		void Execute( SpReal tick, Engine* engine );


		/*!
		   @function  Transition
		   @brief     called when a valid event has been set
		   @return    boost::shared_ptr< GameState >
		   @param     const StateEvent & event
		*/
		boost::shared_ptr< GameState > Transition( const StateEvent& event, Engine* engine );


		/*!
		   @function  IsTransition
		   @brief     determines if the state should transition
		   @return    bool
		   @param     const StateEvent & event - event data
		*/
		bool IsTransition( const StateEvent& event )const;


		/*!
		   @function  SetTransitionState
		   @brief     sets a state and a transition event
		   @return    void
		   @param     boost::shared_ptr< GameState > gameState
		   @param     boost::int32_t eventID
		*/
		void SetTransitionState( boost::shared_ptr< GameState > gameState, boost::int32_t eventID );

		boost::int32_t GetID()const
		{
			return m_id;
		}

	protected:
		GameState( boost::int32_t id );

	private:
		GameState(); 
		

		virtual void DoEnter( Engine* engine ) = 0;
		virtual void DoExecute( SpReal tick, Engine* engine ) = 0;
		virtual void DoTransition( const StateEvent& event, Engine* engine ) = 0;
		virtual bool DoIsTransition( const StateEvent& event )const = 0;

		boost::int32_t m_id;
		std::map< boost::int32_t, boost::shared_ptr< GameState > > m_transList;
	};
}

#endif