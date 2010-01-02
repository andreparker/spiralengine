#include "Engine.hpp"
#include "CoreObject.hpp"
#include "GameObjectHandler.hpp"
#include "CVar.hpp"
#include "FileManager.hpp"
#include "ResourceCatalog.hpp"
#include "Camera.hpp"
#include "Events.hpp"
#include "GameObject.hpp"
#include "ProcessManager.hpp"

#include "../Audio/AudioDriver.hpp"

#include "../Gfx/GfxDriver.hpp"
#include "../Gfx/GfxImpl.hpp"
#include "../Gfx/Color.hpp"
#include "../Gfx/RenderState.hpp"
#include "../Gfx/Texture.hpp"
#include "../Gfx/Utility/Image.hpp"
#include "../Gfx/SpriteLayerImpl.hpp"
#include "../Gfx/SpriteDrawListImpl.hpp"
#include "../Gfx/VisualObjectHandler.hpp"
#include "../Gfx/VisualGameObject.hpp"

#include "GameObjectCreator.hpp"
#include "../Gfx/VisualObjectCreator.hpp"

#include "../Gfx/Font/FreeType/FreeTypeFactory.hpp"
#include "../Gfx/Font/FreeType/FreeTypeFont.hpp"

#include "../Gfx/gui/Gui.hpp"

#include "GameStateMachine.hpp"
#include "GameState.hpp"

#include "Log.hpp"
#include "TypeUtils.hpp"
#include "EventPublisher.hpp"
#include "EventSubscriber.hpp"
#include "GeneralException.hpp"
#include "EngineAttributes.hpp"
#include "UpdateQueue.hpp"

#include <algorithm>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

using namespace Spiral;
using namespace boost;

const std::string version = "^l=================Spiral Engine version 1.0=================\n";
const std::string module = "^lEngine :";

Engine::Engine():
m_stateMachine(),
m_gameObjectList(),
m_visualObjectList(),
m_eventPublisher(),
m_gfxDriver(),
m_audioDriver(),
m_variables(),
m_catolog(),
m_camera(NULL),
m_spriteLayer( NULL ),
m_spriteLayerCount( 0 ),
m_spriteDrawList(),
m_buffer(),
m_attrDirtyFlag(true),
m_eventPublisherThread(),
m_threadManager(),
m_fontFactory(),
m_guiManager(),
m_inputSubscriber(),
m_threadsEnabled(false),
m_modulesCreated(false)
{
}

Engine::~Engine()
{
}

bool Engine::Initialize( shared_ptr< GfxDriver >& gfxDriver,boost::shared_ptr< Audio::AudioDriver >& audioDriver, const any& data ) 
{
	FUNCTION_LOG
	if( gfxDriver )
	{
		LOG_I( version );
		LOG_I( module + " ^wInitializing GfxDriver....\n" );
		bool result0 = gfxDriver->Initialize( data );
		
		LOG_I( module + " ^wInitializing AudioDriver....\n\n" );
		LOG_I( "^y========================================\n" );
		bool result1 = audioDriver->Initialize();
		LOG_I( "^y========================================\n" );

		if( true == result0 && true == result1 )
		{
			m_gfxDriver = gfxDriver;
			m_audioDriver = audioDriver;

			LOG_I( module + " ^wInitializing Engine attributes....\n" );
			InitializeAttributes();

			if( m_modulesCreated == false )
			{
				CreateModules();
				InitEventPublisher();	
				
				LOG_I( module + " ^wInitializing FontFactory....\n" );
				m_fontFactory->Initialize();
				
				LOG_I( module + " ^wInitializing CVars....\n" );
				m_variables->Initialize();
			}
		}
	}

	return bool( m_gfxDriver && m_audioDriver );
}

void Engine::UnInitialize()
{
	if( m_threadsEnabled == true )
	{
		DisableThreads();
	}

	LOG_I( module + "^w Clearing resource catalog....\n" );
	ClearCatalog();

	LOG_I( module + "^w Clearing state machine....\n" );
	m_stateMachine->ClearStates();

	LOG_I( module + "^w Clearing object manager....\n" );
	m_gameObjectList->Clear();

	LOG_I( module + "^w Clearing gui manager....\n" );
	m_guiManager->Clear();

	LOG_I( module + "^w Clearing process manager....\n" );
	m_processManager->Clear();

	LOG_I( module + "^w Uninitializing GfxDriver....\n" );
	m_gfxDriver->UnInitialize();

	LOG_I( module + "^w Uninitializing AudioDriver....\n" );
	m_audioDriver->UnInitialize();
}

void Engine::ClearObjectList()
{
	m_gameObjectList->Clear();
	m_visualObjectList->Clear();
}

void Engine::Tick( SpReal ticks )
{
	UpdateAttributes();
	if( m_buffer.to_ulong() != 0 )
	{
		BufferInfo_t info( m_buffer.to_ulong() );
		m_gfxDriver->ClearBuffer( info );
	}

	if( m_threadsEnabled == false )
	{
		m_eventPublisher->ProcessEventQueue();
		m_audioDriver->UpdateStreamAudioList();
	}


	m_processManager->Tick( ticks );
	m_stateMachine->Tick( ticks, this );
	m_gameObjectList->Tick( ticks );
	m_visualObjectList->Tick( ticks );

	UpdateQueue::instance().Tick( ticks );

	
	BuildSpriteDrawList();
	
	// wait for threads to finish
// 	if( m_threadsEnabled == true )
// 	{
// 		m_threadManager.join_all();
// 	}
	
	DrawSpriteList();

	
	m_guiManager->Present( m_gfxDriver );
	m_gfxDriver->Present();
}

boost::shared_ptr< Engine > Engine::Create()
{
	boost::shared_ptr< Engine > engine( new Engine );
	return engine;
}

void Engine::AddGameState( shared_ptr< GameState >& state, function< void( GameStateMachine* ) > initFunc )
{
	if( state )
	{
		m_stateMachine->AddState( state, initFunc );
	}
}

void Engine::AddVisualGameState( shared_ptr< VisualGameState >& state, function< void( GameStateMachine* ) > initFunc )
{
	if( state )
	{
		m_stateMachine->AddState( state, initFunc );
	}
}

void Engine::InitializeStateMachine()
{
	m_stateMachine->InitializeStates();
}


void Engine::SetCurrentGameState( shared_ptr< GameState >& state )
{
	if( state )
	{
		m_stateMachine->SetCurrentState( state, this );
	}
}

void Engine::AddEventSubscriber( boost::shared_ptr<EventSubscriber>& subscriber )
{
	m_eventPublisher->AddSubscriber( subscriber );
}

void Engine::RemoveEventSubscriber( boost::shared_ptr<EventSubscriber>& subscriber )
{
	m_eventPublisher->RemoveSubscriber( subscriber );
}

boost::shared_ptr< Texture > Engine::LoadTexture( const std::string& fileName, const std::string& TextureName )
{
	LOG_I( module + "^w Loading texture - ^g%1% ^wnameTag - ^g%2% ^w....\n", fileName.c_str(), TextureName.c_str() );
	boost::shared_ptr< Texture > texture;

	// check catalog for texture
	ResourceCatalog::TextureCatalogItr itr = m_catolog->m_textureCatalog.find( TextureName );
	if( itr != m_catolog->m_textureCatalog.end() )
	{
		//found
		texture = (*itr).second;
		LOG_I( module + "^w Texture already exists, grabing it from resource catalog....\n" );
	}else // load it
	{
		shared_ptr< IFile > pngFile;
		if( FileManager::instance().getFile( fileName, pngFile ) )
		{
			File_Auto_Close< IFile > facPng( pngFile );
			shared_ptr< GfxUtil::Image > newImage = GfxUtil::Image::LoadPng( pngFile );
			if( newImage )
			{
				TextureInfo_t info;
				info.bitDepth = newImage->bitDepth;
				info.width    = newImage->width;
				info.height   = newImage->height;
				info.bManaged = false;

				if( m_gfxDriver->CreateTexture( info, texture, newImage->data.get() ) )
				{
					// catalog texture
					CacheTexture( texture, TextureName );
				}
			}else
			{
				LOG_E( module + "^r Error loading texture %1%! \n", fileName.c_str() );
			}
		}
		
	}

	return texture;
}

void Engine::ClearTextureCatalog()
{
	m_catolog->m_textureCatalog.clear();
}

void Engine::ClearCatalog()
{
	ClearTextureCatalog();
	ClearFontCatalog();
	m_catolog.reset( new ResourceCatalog );
}

void Engine::ApplyCamera()
{
	if( NULL != m_camera )
	{
		if( m_camera->IsProjectionDirty() )
		{
			Math::SpMatrix4x4r projection;
			m_camera->GetProjection( projection );
			m_gfxDriver->SetProjection( projection );
		}

		if( m_camera->IsViewDirty() )
		{
			Math::SpMatrix4x4r view;
			m_camera->GetInverseView( view );
			m_gfxDriver->SetView( view );
		}

		m_camera->ResetDirtyFlags();
	}
}

void Engine::ForceApplyCamera()
{
	if( NULL != m_camera )
	{
		Math::SpMatrix4x4r matrix;
		m_camera->GetProjection( matrix );
		m_gfxDriver->SetProjection( matrix );
		m_camera->GetInverseView( matrix );
		m_gfxDriver->SetView( matrix );
		m_camera->ResetDirtyFlags();
	}
}

bool Engine::CreateSpriteLayers( boost::int32_t layerCount )
{
	LOG_D( module + "^w Creating sprite layers - %1%....\n", layerCount );
	shared_array< SpriteLayer > layerCol( new SpriteLayer[ layerCount ] );
	if( layerCol )
	{
		m_spriteLayer = layerCol;
		m_spriteLayerCount = layerCount;
		m_spriteDrawList->SetBuildList( true );
	}

	return bool(layerCol);
}

bool Engine::AddSprite( Sprite* sprite, boost::int32_t layerIndex )
{
	bool bWasAdded = false;
	if( m_spriteLayer )
	{
		LOG_D( module + "^w Adding sprite to layer %1%.\n", layerIndex );
		ValidateLayerIndex( "Engine::AddSprite", layerIndex );
		SpriteLayer::SpriteList& spriteList = m_spriteLayer[ layerIndex ].m_spriteList; 
		SpriteLayer::sprite_itr itr = std::find( spriteList.begin(), spriteList.end(), sprite );
		if( itr == spriteList.end() )
		{
			spriteList.push_back( sprite );
			// build the draw list if the layer is enabled
			if( m_spriteLayer[ layerIndex ].m_enable == true )
			{
				m_spriteDrawList->SetBuildList( true );
			}
			bWasAdded = true;
		}else
		{
			LOG_D( module + "^w Sprite already exist in layer \n" );
		}
	}

	return bWasAdded;
}

bool Engine::RemoveSprite( Sprite* sprite, boost::int32_t layerIndex )
{
	bool bWasRemoved = false;
	if( m_spriteLayer )
	{
		ValidateLayerIndex( "Engine::RemoveSprite", layerIndex );

		LOG_D( module + "^w Removing sprite from layer %1%.\n", layerIndex );
		m_spriteLayer[ layerIndex ].m_spriteList.remove( sprite );
		m_spriteDrawList->SetBuildList( true );
		bWasRemoved = true;
	}

	return bWasRemoved;
}

void Engine::EnableSpriteLayer( boost::int32_t layerIndex, bool bEnable )
{
	if( m_spriteLayer )
	{
		ValidateLayerIndex( "Engine::EnableSpriteLayer", layerIndex );

		LOG_D( module + "^w Enabling sprite layer index %1% Enable:%2%.\n", layerIndex, bEnable );
		if( m_spriteLayer[ layerIndex ].m_enable != bEnable )
		{
			m_spriteLayer[ layerIndex ].m_enable = bEnable;
			m_spriteDrawList->SetBuildList( true );

		}
	}
}

void Engine::ValidateLayerIndex( const std::string& funcName, boost::int32_t layerIndex )
{
	if( layerIndex < 0 || layerIndex >= m_spriteLayerCount )
	{
		THROW_GENERAL_EXCEPTION( funcName + " - Invalid layer index or layers not initialized!" );
	}
}

void Engine::DestroySpriteLayers()
{
	// destroyed the layers
	LOG_D( module + "^w Destroying sprite layers....\n" );
	m_spriteLayer.reset();
	m_spriteLayerCount = 0;
	m_spriteDrawList->ClearList();
}

void Engine::ClearSpriteLayer( boost::int32_t layerIndex )
{
	if( m_spriteLayer )
	{
		ValidateLayerIndex( "Engine::ClearSpriteLayer", layerIndex );
		LOG_D( module + "^wClearing sprite layer %1%.\n",layerIndex);
		m_spriteLayer[ layerIndex ].m_spriteList.clear();
	}
}

void Engine::BuildSpriteDrawList()
{
	if( GetAttr<bool>( EngineAttribute( EngineAttribute::EnableSprites ) ) && 
		m_spriteLayerCount && m_spriteDrawList->NeedsBuild() )
	{
		if( m_threadsEnabled == true )
		{
			LOG_D( module + "^w Creating sprite list build thread.\n" );
			m_threadManager.create_thread( boost::bind( &Engine::SpriteUpdateListThread, this ) );
		}else
		{
			SpriteUpdateListThread();
		}
		
	}
}

void Engine::DrawSpriteList()
{
	
	if( GetAttr<bool>( EngineAttribute( EngineAttribute::EnableSprites ) ) )
	{
		// draw no blended
		if( !m_spriteDrawList->m_opaqueList.empty() )
		{
			m_gfxDriver->Draw( m_spriteDrawList->m_opaqueList );
		}

		// draw blended sprites
		if( !m_spriteDrawList->m_alphaList.empty() )
		{
			m_gfxDriver->SetState( RenderState::Blend( RenderState::RS_TRUE ) );
			m_gfxDriver->Draw( m_spriteDrawList->m_alphaList );
			m_gfxDriver->SetState( RenderState::Blend( RenderState::RS_FALSE ) );
		}
	}
}

void Engine::SetAttribute( const EngineAttribute& attr, const boost::any& value )
{
	m_attributes[ attr.m_attribute ] = value;
	m_attrDirtyFlag = true;
}


void Engine::InitializeAttributes()
{
	m_attributes[ EngineAttribute::ClearColorBuffer ] = any( true );
	m_attributes[ EngineAttribute::ClearDepthBuffer ] = any( true );
	m_attributes[ EngineAttribute::ClearColor ] = any( Rgba() );
	m_attributes[ EngineAttribute::EnableSprites ] = any( true );
	m_attributes[ EngineAttribute::EnableThreading ] = any( false );
	m_attrDirtyFlag = true;
}

const boost::any& Engine::GetAttribute( const EngineAttribute& attr )
{
	return m_attributes[ attr.m_attribute ];
}

void Engine::UpdateAttributes()
{
	if( m_attrDirtyFlag )
	{
		FUNCTION_LOG
		UpdateBufferAttributes();
		UpdateThreadAttributes();
		m_attrDirtyFlag = false;
	}
}

void Engine::UpdateBufferAttributes()
{
	std::bitset<8> bits;
	if( GetAttr<bool>( EngineAttribute(EngineAttribute::ClearColorBuffer) ) )
	{
		Rgba color = GetAttr<Rgba>( EngineAttribute(EngineAttribute::ClearColor) );
		m_gfxDriver->Set( ClearInfoType_t( ClearInfoType_t::ColorValue ), color );
		bits.set( BufferInfo_t::COLOR_BUFFER );
	}
	if( GetAttr<bool>( EngineAttribute(EngineAttribute::ClearDepthBuffer) ) )
	{
		bits.set( BufferInfo_t::DEPTH_BUFFER );
	}

	m_buffer = bits;
}

void Engine::SpriteUpdateListThread()
{
	m_spriteDrawList->ClearList();
	for( int32_t idx = 0; idx < m_spriteLayerCount; ++idx )
	{
		if( m_spriteLayer[ idx ].m_enable == true )
		{
			m_spriteDrawList->AddSpriteList( m_spriteLayer[ idx ].m_spriteList );
		}
	}
	m_spriteDrawList->SetBuildList( false );
}

boost::shared_ptr< Font > Engine::LoadFont( const std::string& fontFile, const std::string& fontName, boost::int32_t charWidth, boost::int32_t charHeight )
{
	boost::shared_ptr< Font > font;
	ResourceCatalog::FontCatalogItr itr = m_catolog->m_fontCatalog.find( fontName );
	LOG_I( module + "^w Loading font file - ^g%1% ^wnameTag - ^g%2% ^wwidth - ^g%3% ^wheight - ^g%4%....\n", fontFile.c_str(), fontName.c_str(), charWidth, charHeight );
	if( itr != m_catolog->m_fontCatalog.end() )
	{
		font = (*itr).second;
		LOG_I( module + "^w Font already exists, grabbing it from catalog.\n" );
	}else
	{
		boost::shared_ptr< IFile > file;
		if( FileManager::instance().getFile( fontFile, file ))
		{
			File_Auto_Close< IFile > fac( file );
			font = m_fontFactory->CreateFont( file, charWidth, charHeight );
			m_catolog->m_fontCatalog[ fontName ] = font;
			m_catolog->m_fontSize += file->Size(); 
			LOG_I( module + "^w Font loaded into catalog.\n" );
			LOG_I( module + "^w Current Font Loaded: ^g%1%^w bytes. \n", m_catolog->m_fontSize );
		}
	}

	return font;
}

void Engine::ClearFontCatalog()
{
	m_catolog->m_fontCatalog.clear();
	m_catolog->m_fontSize = 0;
}

void Engine::CreateModules()
{
	FUNCTION_LOG
	LOG_I( module + " ^wCreating engine modules....\n" );
	m_stateMachine    = make_shared< GameStateMachine >();
	m_gameObjectList  = make_shared< GameObjectHandler >();
	m_visualObjectList= make_shared< VisualObjectHandler >( this );
	m_eventPublisher  = make_shared< EventPublisher >();
	m_variables       = make_shared< CVar >();
	m_spriteDrawList  = make_shared< SpriteDrawList >();
	m_fontFactory     = make_shared< FreeTypeFactory >();
	m_guiManager      = make_shared< GUI::GuiManager >( this );
	m_processManager  = make_shared< ProcessManager >();
	m_inputSubscriber = make_shared< EventSubscriber >( Event( Event::EVENT_ANY, Catagory_Input::value ) );
	m_catolog.reset( new ResourceCatalog );
	m_modulesCreated = true;
}

void Engine::SetGfxValues()
{
	FUNCTION_LOG
	int32_t vidWidth = m_variables->GetVarValue( "vid_width", EmptyType<int32_t>() );
	int32_t vidHeight = m_variables->GetVarValue( "vid_height", EmptyType<int32_t>() );
	int32_t vidFullscreen = m_variables->GetVarValue( "vid_fullscreen", EmptyType<int32_t>() );

	GfxVidInfo_t vidInfo;
	vidInfo.bitDepth = 32;
	vidInfo.height = vidHeight;
	vidInfo.width  = vidWidth;

	LOG_I( module + " ^wSetting Gfx defaults....\n" );
	m_gfxDriver->SetViewPort( 0, 0, vidWidth, vidHeight );
	m_gfxDriver->SetVideo( vidInfo, (vidFullscreen == 1) ? true : false );
	m_gfxDriver->Set( ClearInfoType_t( ClearInfoType_t::ColorValue ), Rgba( 0.3f, 0.3f, 0.3f ) );
	m_gfxDriver->ClearBuffer( BufferInfo_t( BIT_ON2( BufferInfo_t::COLOR_BUFFER, BufferInfo_t::DEPTH_BUFFER ) ) );

	m_gfxDriver->SetState( RenderState::Cull_Face( RenderState::RS_TRUE ) );
	m_gfxDriver->SetState( RenderState::Depth_Test( RenderState::RS_TRUE ) );
}

void Engine::InitEventPublisher()
{
	m_eventPublisher->AddSubscriber( m_inputSubscriber );
	m_inputSubscriber->AddCallback( boost::bind( &GUI::GuiManager::Input, m_guiManager, _1, _2 ) );
}

bool Engine::UnCacheFont( const std::string& fontName )
{
	bool found = false;
	ResourceCatalog::FontCatalogItr font = m_catolog->m_fontCatalog.find( fontName );

	if( font != m_catolog->m_fontCatalog.end() )
	{
		m_catolog->m_fontCatalog.erase( font );
		found = true;
	}

	return found;
}

bool Engine::UnCacheTexture( const std::string& textureName )
{
	bool found = false;
	ResourceCatalog::TextureCatalogItr texture = m_catolog->m_textureCatalog.find( textureName );

	if( texture != m_catolog->m_textureCatalog.end() )
	{
		shared_ptr< Texture > tex = texture->second;
		m_catolog->m_textureSize -= tex->Size();
		m_catolog->m_textureCatalog.erase( texture );
		found = true;
	}

	return found;
}

void Engine::EnableThreads()
{
	LOG_I( module + " ^wCreating EventPublisher thread....\n" );
	m_eventPublisher->ClearEventQueue();
	m_eventPublisherThread = boost::thread( &EventPublisher::ProcessEventQueueThreaded, ref( m_eventPublisher ) );
}

void Engine::DisableThreads()
{
	LOG_I( module + " ^wDestroying EventPublisher thread....\n" );
	m_eventPublisherThread.detach();
	m_eventPublisherThread.join();
	m_eventPublisher->ClearEventQueue();
}

void Engine::UpdateThreadAttributes()
{
	if( m_threadsEnabled == false && GetAttr<bool>( EngineAttribute(EngineAttribute::EnableThreading) ) == true )
	{
		UpdateQueue::instance().Add( bind( &Engine::EnableThreads, this ) );
		m_threadsEnabled = true;
	}else if( m_threadsEnabled == true && GetAttr<bool>( EngineAttribute(EngineAttribute::EnableThreading) ) == false )
	{
		UpdateQueue::instance().Add( bind( &Engine::DisableThreads, this ) );
		m_threadsEnabled = false;
	}
}

void Engine::ScreenToWorld( const Math::SpVector2r& scr_pos, Math::SpVector2r& world )
{
	if( m_camera )
	{
		Math::SpMatrix4x4r proj;
		Math::SpMatrix4x4r view;

		m_camera->GetProjection(proj);
		m_camera->GetView(view);

		Math::SpVector3r pos = Math::make_vector( scr_pos[0], scr_pos[1], -1.0f );

		Rect<boost::int32_t> viewPort;
		GetGfxDriver()->GetViewPort( viewPort );
		Math::UnProject( pos, (view * proj), Rect<SpReal>( viewPort ), pos );

		world[0] = pos[0];
		world[1] = pos[1];
	}else
	{
		THROW_GENERAL_EXCEPTION( module + "Engine::ScreenToWorld - Error: no camera set!" );
	}
}

void Engine::WorldToScreen( const Math::SpVector2r& world_pos, Math::SpVector2r& scr_pos )
{
	if( m_camera )
	{
		Math::SpMatrix4x4r proj;
		Math::SpMatrix4x4r view;

		m_camera->GetProjection(proj);
		m_camera->GetView(view);

		Math::SpVector3r pos = Math::make_vector( world_pos[0], world_pos[1], -1.0f );

		Rect<boost::int32_t> viewPort;
		GetGfxDriver()->GetViewPort( viewPort );
		Math::Project( pos, (view * proj), Rect<SpReal>( viewPort ), pos );

		scr_pos[0] = pos[0];
		scr_pos[1] = pos[1];
	}else
	{
		THROW_GENERAL_EXCEPTION( module + "Engine::WorldToScreen - Error: no camera set!" );
	}
}

bool Engine::LoadConfig( const std::string& fileName )
{
	bool cfgLoaded = false;
	boost::shared_ptr< IFile > configFile;
	if( FileManager::instance().getFile( fileName, configFile ) )
	{
		File_Auto_Close< IFile > ac( configFile );
		m_variables->ProcessFile( configFile );
		SetGfxValues();
		cfgLoaded = true;
	}

	return cfgLoaded;
}

bool Engine::CacheTexture( const boost::shared_ptr< Texture >& texture, const std::string& textureName )
{
	bool textureCached = false;

	LOG_I( module + " ^wAttempting to cache ^g%1% ^wtexture...\n", textureName );
	ResourceCatalog::TextureCatalogItr itr = m_catolog->m_textureCatalog.find( textureName );

	if( itr == m_catolog->m_textureCatalog.end() )
	{
		m_catolog->m_textureCatalog.insert( std::pair< std::string, boost::shared_ptr< Texture > >( textureName, texture ) );
		m_catolog->m_textureSize += texture->Size();

		if( m_catolog->m_textureSize > (1024*1024) )
		{
			SpReal megaByte = m_catolog->m_textureSize / (1024.0f * 1024.0f );
			LOG_I( module + " ^wTexture now cached ^g%1% ^wMB.\n", megaByte );

		}else if( m_catolog->m_textureSize > 1024 )
		{
			SpReal kiloByte = m_catolog->m_textureSize / 1024.0f;
			LOG_I( module + " ^wTexture now cached ^g%1% ^wKB.\n", kiloByte );
		}else
		{
			LOG_I( module + " ^wTexture now cached ^g%1% ^wbytes.\n", m_catolog->m_textureSize );
		}
		textureCached = true;
	}else
	{
		LOG_I( module + " ^rTexture already cached!\n" );
	}

	return textureCached;
}

VisualGameObject* Engine::CreateVisual( const char* gameObjectType )
{
	return VisualObjectCreator::instance().CreateFromGameObjectName( gameObjectType );
}

void Engine::StoreObjects( const boost::shared_ptr<GameObject>& obj, const boost::shared_ptr<VisualGameObject>& vis )
{
	if( vis )
	{
		vis->SetGameObject( obj.get() );
		m_visualObjectList->Add( vis );
	}

	m_gameObjectList->Add( obj );
}

