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
	class VisualGameState : private boost::noncopyable
	{
	protected:
		VisualGameState( boost::int32_t id );
	public:
		virtual ~VisualGameState();

		void Enter();
		void Execute( real tick );
		void Transition();

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
		virtual void DoEnter() = 0;
		virtual void DoExecute( real tick ) = 0;
		virtual void DoTransition() = 0;
	};
}

#endif