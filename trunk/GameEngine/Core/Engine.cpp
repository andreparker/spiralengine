#include "Engine.hpp"
#include "CoreObject.hpp"
#include "GameObjectHandler.hpp"
#include "CVar.hpp"
#include "FileManager.hpp"
#include "ResourceCatalog.hpp"
#include "Camera.hpp"

#include "../Gfx/GfxDriver.hpp"
#include "../Gfx/GfxImpl.hpp"
#include "../Gfx/Color.hpp"
#include "../Gfx/RenderState.hpp"
#include "../Gfx/Texture.hpp"
#include "../Gfx/Utility/Image.hpp"
#include "../Gfx/SpriteLayerImpl.hpp"
#include "../Gfx/SpriteDrawListImpl.hpp"

#include "../Gfx/Font/FreeType/FreeTypeFactory.hpp"
#include "../Gfx/Font/FreeType/FreeTypeFont.hpp"

#include "GameStateMachine.hpp"
#include "GameState.hpp"

#include "TypeUtils.hpp"
#include "EventPublisher.hpp"
#include "EventSubscriber.hpp"
#include "GeneralException.hpp"
#include "EngineAttributes.hpp"

#include <algorithm>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

using namespace Spiral;
using namespace boost;

Engine::Engine():
m_stateMachine(),
m_gameObjectList(),
m_eventPublisher(),
m_gfxDriver(),
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
m_fontFactory()
{
}

Engine::~Engine()
{
}

bool Engine::Initialize( shared_ptr< GfxDriver >& gfxDriver, any& data ) 
{
	if( gfxDriver )
	{
		bool result = gfxDriver->Initialize( data );
		if( true == result )
		{
			m_gfxDriver      = gfxDriver;
			m_stateMachine   = make_shared< GameStateMachine >();
			m_gameObjectList = make_shared< GameObjectHandler >();
			m_eventPublisher = make_shared< EventPublisher >();
			m_variables      = make_shared< CVar >();
			m_spriteDrawList = make_shared< SpriteDrawList >();
			m_fontFactory    = make_shared< FreeTypeFactory >();

			m_eventPublisherThread = boost::thread( &EventPublisher::ProcessEventQueueThreaded, m_eventPublisher );
			m_catolog.reset( new ResourceCatalog );

			InitializeAttributes();
			m_fontFactory->Initialize();

			m_variables->Initialize();
			{
				boost::shared_ptr< IFile > configFile;
				if( FileManager::instance().getFile( "Data/Config/config.cfg", configFile ) )
				{
					File_Auto_Close< IFile > ac( configFile );
					m_variables->ProcessFile( configFile );
				}
			}

			int32_t vidWidth = m_variables->GetVarValue( "vid_width", EmptyType<int32_t>() );
			int32_t vidHeight = m_variables->GetVarValue( "vid_height", EmptyType<int32_t>() );
			int32_t vidFullscreen = m_variables->GetVarValue( "vid_fullscreen", EmptyType<int32_t>() );

			GfxVidInfo_t vidInfo;
			vidInfo.bitDepth = 32;
			vidInfo.height = vidHeight;
			vidInfo.width  = vidWidth;

			m_gfxDriver->SetViewPort( 0, 0, vidWidth, vidHeight );
			m_gfxDriver->SetVideo( vidInfo, (vidFullscreen == 1) ? true : false );
			m_gfxDriver->Set( ClearInfoType_t( ClearInfoType_t::ColorValue ), Rgba( 0.3f, 0.3f, 0.3f ) );
			m_gfxDriver->ClearBuffer( BufferInfo_t( BIT_ON2( BufferInfo_t::COLOR_BUFFER, BufferInfo_t::DEPTH_BUFFER ) ) );

			m_gfxDriver->SetState( RenderState::Cull_Face( RenderState::RS_TRUE ) );
			m_gfxDriver->SetState( RenderState::Depth_Test( RenderState::RS_TRUE ) );
		}
	}

	return bool( m_gfxDriver );
}

void Engine::UnInitialize()
{
	m_eventPublisherThread.detach();
	ClearCatalog();
}

void Engine::AddGameObject( shared_ptr< CoreObject >& object )
{
	if( object )
	{
		m_gameObjectList->Add( object );
	}
}

void Engine::ClearObjectList()
{
	m_gameObjectList->Clear();
}

void Engine::Tick( SpReal ticks )
{
	UpdateAttributes();
	if( m_buffer.to_ulong() != 0 )
	{
		BufferInfo_t info( m_buffer.to_ulong() );
		m_gfxDriver->ClearBuffer( info );
	}
	
	m_stateMachine->Tick( ticks, this );
	m_gameObjectList->Tick( ticks );
	BuildSpriteDrawList();
	
	// wait for threads to finish
	m_threadManager.join_all();

	DrawSpriteList();
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
	boost::shared_ptr< Texture > texture;

	// check catalog for texture
	ResourceCatalog::TextureCatalogItr itr = m_catolog->m_textureCatalog.find( TextureName );
	if( itr != m_catolog->m_textureCatalog.end() )
	{
		//found
		texture = (*itr).second;
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
					m_catolog->m_textureCatalog[ TextureName ] = texture;
				}
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
		throw GeneralException( funcName + " - Invalid layer index or layers not initialized!" );
	}
}

void Engine::DestroySpriteLayers()
{
	// destroyed the layers
	m_spriteLayer.reset();
	m_spriteLayerCount = 0;
	m_spriteDrawList->ClearList();
}

void Engine::ClearSpriteLayer( boost::int32_t layerIndex )
{
	if( m_spriteLayer )
	{
		ValidateLayerIndex( "Engine::ClearSpriteLayer", layerIndex );
		m_spriteLayer[ layerIndex ].m_spriteList.clear();
	}
}

void Engine::BuildSpriteDrawList()
{
	if( GetAttr<bool>( EngineAttribute( EngineAttribute::EnableSprites ) ) && 
		m_spriteLayerCount && m_spriteDrawList->NeedsBuild() )
	{
		m_threadManager.create_thread( boost::bind( &Engine::SpriteUpdateListThread, this ) );
	}
}

void Engine::DrawSpriteList()
{
	// draw no blended
	if( GetAttr<bool>( EngineAttribute( EngineAttribute::EnableSprites ) ) )
	{
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
	m_attrDirtyFlag = true;
}

boost::any Engine::GetAttribute( const EngineAttribute& attr )
{
	return m_attributes[ attr.m_attribute ];
}

void Engine::UpdateAttributes()
{
	if( m_attrDirtyFlag )
	{
		UpdateBufferAttributes();
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
	if( itr != m_catolog->m_fontCatalog.end() )
	{
		font = (*itr).second;
	}else
	{
		boost::shared_ptr< IFile > file;
		if( FileManager::instance().getFile( fontFile, file ))
		{
			File_Auto_Close< IFile > fac( file );
			font = m_fontFactory->CreateFont( file, charWidth, charHeight );
			m_catolog->m_fontCatalog[ fontName ] = font;
		}
	}

	return font;
}

void Engine::ClearFontCatalog()
{
	m_catolog->m_fontCatalog.clear();
}