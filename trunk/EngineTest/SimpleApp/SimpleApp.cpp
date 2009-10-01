#include "SimpleApp.hpp"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <string>

using namespace SimpleApp;
using namespace Spiral;
using namespace boost;

bool App::DoInit( boost::int32_t /*argc*/, std::list< boost::shared_array< char > >& /*argList*/, boost::shared_ptr< Engine >& engine )
{
	// cache of the engine pointer
	m_engine = engine;

	shared_ptr< Texture > texture = engine->LoadTexture( "Data/ball32.png", "Ball" );
	shared_ptr< Texture > alpha_texture = engine->LoadTexture( "Data/ball32_alpha.png", "Ball_alpha" );
	shared_ptr< GfxDriver > gfxDriver = engine->GetGfxDriver();


	gfxDriver->CreateSprite( m_sprite, texture, Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), Rect< SpReal >( 16.0f, 32.0f, 32.0f, 16.0f ));
	gfxDriver->CreateSprite( m_sprite_alpha, alpha_texture, Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), Rect< SpReal >( 16.0f, 32.0f, 32.0f, 16.0f ));
	m_sprite_alpha->SetPosition( 50.0f, 50.0f );
	m_sprite_alpha->SetAlphaBlend( true );
	
	m_arialN = engine->LoadFont( "c:/windows/fonts/arialn.ttf", "arial_n", 16, 16 );
	int32_t width = 1024,height = 1024;
	std::string text = "Testing Font rendering code using true type font.\nThis font is arial narrow.\nTesting newline code";
	m_arialN->CalcSurfaceSize( text, width, height );
	shared_ptr< Surface > surf = make_surface( width, height, 3 );
	m_arialN->RenderOpaque( surf, text, Rgba( 0.0f, 1.0f, 0.0f ) );


	shared_ptr< Texture > fontTexture = surf->CreateTextureFromData( gfxDriver );

	gfxDriver->CreateSprite( m_fontSprite, fontTexture, Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), Rect< SpReal >( 32.0f, width*2, height*2, 8.0f ) );
	m_fontSprite->SetPosition( 400.0f, 500.0f );
	//m_fontSprite->SetAlphaBlend( true );
	

	gfxDriver->SetState( RenderState::Depth_Test( RenderState::RS_FALSE ) );
	gfxDriver->SetState( RenderState::Texture( RenderState::RS_TRUE ) );
	gfxDriver->SetState( RenderState::Cull_Face( RenderState::RS_FALSE ) );

	if( engine->CreateSpriteLayers( 1 ) )
	{
		engine->AddSprite( m_sprite.get(), 0 );
		engine->AddSprite( m_sprite_alpha.get(), 0 );
		engine->AddSprite( m_fontSprite.get(), 0 );
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

	return true;
}

bool App::DoUnInit()
{
	delete m_camera;
	m_sprite.reset();
	m_sprite_alpha.reset();
	m_keyDownSubscriber.reset();
	m_engine.reset(); // release the reference to the engine (IMPORTANT)
	m_arialN.reset();
	m_fontSprite.reset();

	return true;
}

SP_DECL_APPLICATION( SimpleApp::App )

App::App():
m_sprite(),
m_sprite_alpha(),
m_keyDownSubscriber(),
m_engine(),
m_arialN(),
m_fontSprite(),
m_camera(NULL)
{

}

void App::KeyDown( const Event& event, const any& data )
{
	int32_t key = any_cast<int32_t>(data);
	if( key == 27 )
	{
		// close the app
		m_engine->GetEventPublisher()->Publish( Event( event_AppStatus_shutdown, Catagory_App_Status::value ), any() );
	}
}