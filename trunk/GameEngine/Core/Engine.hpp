/*!
*/
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/function.hpp>

#include "Sp_DataTypes.hpp"

namespace Spiral
{
	class GameState;
	class GameStateMachine;
	class GameObjectHandler;
	class VisualGameState;
	class CoreObject;
	class GfxDriver;

	class Engine : private boost::noncopyable
	{
	public:
		~Engine();

		/*!
		   @function  Initialize
		   @brief     initializes the game engine
		   @return    bool
		   @param     boost::shared_ptr< GfxDriver >& gfxDriver
		   @param     boost::any & data - data to initialize the engine
		*/
		bool Initialize( boost::shared_ptr< GfxDriver >& gfxDriver, boost::any& data );

		/*!
		   @function  InitializeStateMachine
		   @brief     initializes state machine, call this after adding all your states
		   @return    void
		*/
		void InitializeStateMachine();


		/*!
		   @function  UnInitialize
		   @brief     shuts down the game engine
		   @return    void
		*/
		void UnInitialize();


		/*!
		   @function  Tick
		   @brief     called every frame to update components
		   @return    void
		   @param     real ticks
		*/
		void Tick( real ticks );

		/*!
		   @function  AddGameObject
		   @brief     add a game object to the engine
		   @return    void
		   @param     boost::shared_ptr< CoreObject > & object
		*/
		void AddGameObject( boost::shared_ptr< CoreObject >& object );

		/*!
		   @function  AddGameState
		   @brief     add a game state to the state machine
		   @return    void
		   @param     boost::shared_ptr< GameState > & state
		   @param     boost::function< void(GameStateMachine *) > initFunc
		*/
		void AddGameState( boost::shared_ptr< GameState >& state, boost::function< void( GameStateMachine* ) > initFunc );
		void AddVisualGameState( boost::shared_ptr< VisualGameState >& state, boost::function< void( GameStateMachine* ) > initFunc );

		/*!
		   @function  SetCurrentGameState
		   @brief     sets the current game state
		   @return    void
		   @param     boost::shared_ptr< GameState > & state
		*/
		void SetCurrentGameState( boost::shared_ptr< GameState >& state );

		/*!
		   @function  ClearObjectList
		   @brief     clear the object list
		   @return    void
		*/
		void ClearObjectList();

		/*!
		   @function  Create
		   @brief     creates a instance of engine, not singleton
		   @return    boost::shared_ptr< Engine >
		*/
		static boost::shared_ptr< Engine > Create();

	private:
		boost::shared_ptr< GameStateMachine > m_stateMachine;
		boost::shared_ptr< GameObjectHandler > m_gameObjectList;
		boost::shared_ptr< GfxDriver > m_gfxDriver;
		Engine();
	};
}

#endif