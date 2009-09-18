#include "SimpleApp.hpp"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

using namespace SimpleApp;
using namespace Spiral;
using namespace boost;

bool App::DoInit( boost::int32_t /*argc*/, std::list< boost::shared_array< char > >& /*argList*/, boost::shared_ptr< Engine >& engine )
{
	
	shared_ptr< Texture > texture = engine->LoadTexture( "Data/ball32.png", "Ball" );
	shared_ptr< Texture > alpha_texture = engine->LoadTexture( "Data/ball32_alpha.png", "Ball_alpha" );

	shared_ptr< GfxDriver > gfxDriver = engine->GetGfxDriver();
	gfxDriver->CreateSprite( m_sprite, texture, Rect< SpReal >( 0.0f, 1.0f, 0.0f, 1.0f ), Rect< SpReal >( 16.0f, 32.0f, 32.0f, 16.0f ));
	gfxDriver->CreateSprite( m_sprite_alpha, alpha_texture, Rect< SpReal >( 0.0f, 1.0f, 0.0f, 1.0f ), Rect< SpReal >( 16.0f, 32.0f, 32.0f, 16.0f ));
	
	m_sprite_alpha->SetPosition( 50.0f, 50.0f );
	m_sprite_alpha->SetAlphaBlend( true );

	gfxDriver->SetState( RenderState::Depth_Test( RenderState::RS_FALSE ) );
	gfxDriver->SetState( RenderState::Texture( RenderState::RS_TRUE ) );
	gfxDriver->SetState( RenderState::Cull_Face( RenderState::RS_FALSE ) );

	if( engine->CreateSpriteLayers( 1 ) )
	{
		engine->AddSprite( m_sprite.get(), 0 );
		engine->AddSprite( m_sprite_alpha.get(), 0 );
		engine->EnableSpriteLayer( 0, true );
	}

	m_keyDownSubscriber = make_shared< EventSubscriber >( Event( Event::EVENT_ANY, Catagory_KeyBoard::value ) );
	m_keyDownSubscriber->AddCallback( boost::bind( &App::KeyDown, this, _1, _2 ) );
	engine->GetEventPublisher()->AddSubscriber( m_keyDownSubscriber );

	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearColor ), Rgba( 1.0f, 0.0f , 0.0f ) );
	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearColorBuffer ), true );
	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearDepthBuffer ), false );

	
	m_camera = new Camera( Math::SpVector3r( 0.0f, 0.0f, 0.0f ) );
	m_camera->SetProjection( Projection::CreateOrtho2D( 0.0f , 0.0f , 1024.0f , 768.0f ) );
	engine->SetCamera( m_camera );
	return true;
}

bool App::DoRun( SpReal ticks, boost::shared_ptr< Engine >& engine )
{
	static SpReal angle = 0.0f;
	engine->ApplyCamera();

	m_sprite->SetAngle( angle );
	angle += 2.0f * (ticks*60.0f); // fixed 60 times a second
	m_sprite_alpha->SetAngle( angle );

	if( angle > 600.0f )
	{
		engine->GetEventPublisher()->Publish( Event( event_AppStatus_shutdown, Catagory_App_Status::value ), any() );
	}

	return true;
}

bool App::DoUnInit()
{
	delete m_camera;
	m_sprite.reset();
	m_sprite_alpha.reset();
	m_keyDownSubscriber.reset();

	return true;
}

SP_DECL_APPLICATION( SimpleApp::App )

App::App():
m_sprite(),
m_sprite_alpha(),
m_keyDownSubscriber(),
m_camera(NULL)
{

}

void App::KeyDown( const Event& event, const any& data )
{
	int32_t key = any_cast<int32_t>(data);
}