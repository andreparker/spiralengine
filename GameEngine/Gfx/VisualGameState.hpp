/*!
*/
#ifndef VISUAL_GAME_STATE_HPP
#define VISUAL_GAME_STATE_HPP

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include "../Core/Sp_DataTypes.hpp"

namespace Spiral
{

	class GameState;
	class Engine;
	class VisualGameState : private boost::noncopyable
	{
	protected:
		/*!
		   @function  VisualGameState
		   @brief     constructs the game state with a gameState id
		   @return    none
		   @param     boost::int32_t id
		*/
		VisualGameState( boost::int32_t id );
	public:
		virtual ~VisualGameState();

		void Enter( Engine* engine );
		void Execute( SpReal tick, Engine* engine );
		void Transition( Engine* engine );

		void Attach( boost::shared_ptr< GameState >& state );

		boost::int32_t GetID()const
		{
			return m_id;
		}

	private:
		VisualGameState();

		boost::shared_ptr< GameState > m_gameState;	// matching game state
		boost::int32_t m_id;

	private:
		virtual void DoEnter( Engine* engine ) = 0;
		virtual void DoExecute( SpReal tick, Engine* engine ) = 0;
		virtual void DoTransition( Engine* engine ) = 0;
	};
}

#endif