/*!
*/
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <bitset>
#include <boost/any.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <boost/thread.hpp>

#include "Sp_DataTypes.hpp"
#include "../Gfx/SpriteLayerImplFwd.hpp"
#include "../Gfx/SpriteDrawListImplFwd.hpp"

namespace Spiral
{
	class Camera;
	class GameState;
	class GameStateMachine;
	class GameObjectHandler;
	class VisualGameState;
	class CoreObject;
	class EventPublisher;
	class EventSubscriber;
	class CVar;

	class Texture;
	class GfxDriver;
	class Sprite;
	struct ResourceCatalog;
	struct EngineAttribute;

	class Engine : private boost::noncopyable
	{
	public:
		~Engine();

		void SetAttribute( const EngineAttribute& attr, const boost::any& value );
		boost::any GetAttribute( const EngineAttribute& attr );

		/*!
		   @function  CreateSpriteLayers
		   @brief     create layers for sprites, 0 - layerCount-1
		   @return    bool
		   @param     boost::int32_t layerCount
		*/
		bool CreateSpriteLayers( boost::int32_t layerCount );

		/*!
		   @function  AddSprite
		   @brief     adds a sprite to a specific layer
		   @return    bool
		   @param     Sprite * sprite
		   @param     boost::int32_t layerIndex
		*/
		bool AddSprite( Sprite* sprite, boost::int32_t layerIndex );

		/*!
		   @function  RemoveSprite
		   @brief     removes a sprite from a list on the layer
		   @return    bool
		   @param     Sprite * sprite
		   @param     boost::int32_t layerIndex
		*/
		bool RemoveSprite( Sprite* sprite, boost::int32_t layerIndex );

		/*!
		   @function  EnableSpriteLayer
		   @brief     enable/disable a layer to not be drawn
		   @return    void
		   @param     boost::int32_t layerIndex
		   @param     bool bEnable
		*/
		void EnableSpriteLayer( boost::int32_t layerIndex, bool bEnable );

		/*!
		   @function  ClearSpriteLayer
		   @brief     clears a layer of its sprites
		   @return    void
		   @param     boost::int32_t layerIndex
		*/
		void ClearSpriteLayer( boost::int32_t layerIndex );

		/*!
		   @function  DestroySpriteLayers
		   @brief     destroys all layers
		   @return    void
		*/
		void DestroySpriteLayers();

		void ApplyCamera();
		/*!
		   @function  SetCamera
		   @brief     sets a camera view
		   @return    void
		   @param     Camera * camera
		*/
		void SetCamera( Camera* camera )
		{
			m_camera = camera;
			ForceApplyCamera();
		}

		const Camera* GetCamera()const
		{
			return m_camera;
		}

		Camera* GetCamera()
		{
			return const_cast< Camera* >( static_cast< const Engine* >( this )->GetCamera() );
		}

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
		   @param     SpReal ticks
		*/
		void Tick( SpReal ticks );

		/*!
		   @function  AddEventSubscriber
		   @brief     adds a class subscribed to a event
		   @return    void
		   @param     boost::shared_ptr<EventSubscriber> & subscriber
		*/
		void AddEventSubscriber( boost::shared_ptr<EventSubscriber>& subscriber );

		/*!
		   @function  RemoveEventSubscriber
		   @brief     removes a subscription
		   @return    void
		   @param     boost::shared_ptr<EventSubscriber> & subscriber
		*/
		void RemoveEventSubscriber( boost::shared_ptr<EventSubscriber>& subscriber );

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

		boost::shared_ptr< GfxDriver > GetGfxDriver()const
		{
			return m_gfxDriver;
		}

		boost::shared_ptr< CVar > GetVariables()const
		{
			return m_variables;
		}

		/*!
		   @function  LoadTexture
		   @brief     loads a png image and returns a texture
		   @return    boost::shared_ptr< Texture >
		   @param     const std::string & fileName - png filename
		   @param     const std::string & TextureName - texture name for id
		*/
		boost::shared_ptr< Texture > LoadTexture( const std::string& fileName, const std::string& TextureName );

		boost::shared_ptr< EventPublisher > GetEventPublisher()const
		{
			return m_eventPublisher;
		}
	private:
		
		/*!
		   @function  ClearTextureCatalog
		   @brief     clears and resets the texture catalog
		   @return    void
		*/
		void ClearTextureCatalog();

		/*!
		   @function  ClearCatalog
		   @brief     clears all catalogs
		   @return    void
		*/
		void ClearCatalog();

		/*!
		   @function  ForceApplyCamera
		   @brief     applys the camera without checking its dirty flag
		   @return    void
		*/
		void ForceApplyCamera();

		/*!
		   @function  ValidateLayerIndex
		   @brief     spits out a exception if layerIndex is invalid
		   @return    void
		   @param     const std::string & funcName - the function that is using layerIndex
		   @param     boost::int32_t layerIndex - index into sprite layers
		*/
		void ValidateLayerIndex( const std::string& funcName, boost::int32_t layerIndex );

		void BuildSpriteDrawList();

		void DrawSpriteList();

		/*!
		   @function  InitializeAttributes
		   @brief     initializes the engines attributes
		   @return    void
		*/
		void InitializeAttributes();

		/*!
		   @function  UpdateAttributes
		   @brief     called once a frame to update changed attributes
		   @return    void
		*/
		void UpdateAttributes();

		/*!
		   @function  UpdateBufferAttributes
		   @brief     update buffer attributes such as clearing the colorbuffer/zbuffer/stencil etc
		   @return    void
		*/
		void UpdateBufferAttributes();

		/*!
		   @function  SpriteUpdateListThread
		   @brief     updates the sprites list in a thread
		   @return    void
		*/
		void SpriteUpdateListThread();

		template< class T >
		const T GetAttr( const EngineAttribute& attr )const
		{
			return boost::any_cast< const T >( m_attributes[ attr.m_attribute ] );
		}
	private:
		static const boost::int32_t AttributeCount = 10;

		boost::shared_ptr< GameStateMachine > m_stateMachine;
		boost::shared_ptr< GameObjectHandler > m_gameObjectList;
		boost::shared_ptr< EventPublisher > m_eventPublisher;
		boost::shared_ptr< GfxDriver > m_gfxDriver;
		boost::shared_ptr< CVar > m_variables;
		boost::scoped_ptr< ResourceCatalog > m_catolog;
		Camera* m_camera;
		boost::shared_array< SpriteLayer > m_spriteLayer;
		boost::int32_t m_spriteLayerCount;
		boost::shared_ptr< SpriteDrawList > m_spriteDrawList;
		std::bitset< 8 > m_buffer;
		bool m_attrDirtyFlag;
		boost::thread m_eventPublisherThread;
		boost::thread_group m_threadManager;
		boost::any m_attributes[ AttributeCount ];

		Engine();
	};
}

#endif