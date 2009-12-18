#include "SimpleApp.hpp"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
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
	shared_ptr< Texture > button_texture = engine->LoadTexture( "Data/GUI/def_button.png", "Button_texture" );
	shared_ptr< Texture > window_texture = engine->LoadTexture( "Data/GUI/def_window.png", "Window_texture" );
	shared_ptr< Texture > check_boxTexture = engine->LoadTexture( "Data/GUI/def_check.png", "check_texture" );
	shared_ptr< Texture > slider_bgTexture = engine->LoadTexture( "Data/GUI/def_slider_back.png", "slider_bk_texture" );
	shared_ptr< Texture > slider_texture = engine->LoadTexture( "Data/GUI/def_slider.png", "slider_texture" );
	shared_ptr< GfxDriver > gfxDriver = engine->GetGfxDriver();


	gfxDriver->CreateSprite( m_sprite, texture, Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), Rect< SpReal >( 16.0f, 32.0f, 32.0f, 16.0f ));
	gfxDriver->CreateSprite( m_sprite_alpha, alpha_texture, Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), Rect< SpReal >( 16.0f, 32.0f, 32.0f, 16.0f ));
	m_sprite_alpha->SetPosition( 50.0f, 50.0f );
	m_sprite_alpha->SetAlphaBlend( true );
	
	m_arialN = engine->LoadFont( "c:/windows/fonts/arialn.ttf", "arial_n", 16, 10 );
	SpString text = L"Testing Font rendering code using true type font.\nThis font is arial narrow.\nTesting newline code";


	shared_ptr< GUI::GuiText > guiText = make_shared< GUI::GuiText >( Math::make_vector<SpReal>(32.0f, 100.0f),gfxDriver,Rgba(1.0f,1.0f,1.0f),text.length()+1,m_arialN,text,true );
	shared_ptr< GUI::GuiText > guiButtonText1 = make_shared< GUI::GuiText >( Math::make_vector<SpReal>(8.0f, 10.0f),gfxDriver,Rgba(1.0f,1.0f,1.0f),10,m_arialN,L"Button 1" );
	shared_ptr< GUI::GuiText > guiButtonText2 = make_shared< GUI::GuiText >( Math::make_vector<SpReal>(8.0f, 10.0f),gfxDriver,Rgba(1.0f,1.0f,1.0f),10,m_arialN,L"Button 2" );

	guiButtonText1->AllowFocus(false);
	guiButtonText2->AllowFocus(false);
	guiButtonText1->Show();
	guiButtonText2->Show();

	guiText->AllowFocus(false);
	guiText->Show();
	
	m_button = make_shared< GUI::GuiButton >( Math::make_vector( 128.0f,450.0f), Rect< SpReal >( 0, 64, 32, 0 ), 
		Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), button_texture, true );
	m_button->AddChild( guiButtonText2 );
	m_button->Show();

	shared_ptr< GUI::GuiButton > button = make_shared< GUI::GuiButton >( Math::make_vector( 32.0f ,450.0f ), Rect< SpReal >( 0, 64, 32, 0 ), 
		Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), button_texture, true );
	button->AddChild( guiButtonText1 );
	button->Show();
	

	shared_ptr< GUI::GuiEditBox > editbox = make_shared< GUI::GuiEditBox >( Math::make_vector( 32.0f,332.0f ), gfxDriver, 
		Rgba( 1.0f, 1.0f, 1.0f ), Rgba(), m_arialN, 32, L"Edit box 2" );
	shared_ptr< GUI::GuiEditBox > editbox1 = make_shared< GUI::GuiEditBox >( Math::make_vector( 32.0f,300.0f ), gfxDriver, 
		Rgba( 1.0f, 1.0f, 1.0f ), Rgba(), m_arialN, 32, L"Edit box 1" );
	button->ConnectHandler( GUI::button_Press, bind( &App::ButtonPress, this, _1, _2, _3 ) );

	m_window = make_shared< GUI::GuiWindow >( Math::make_vector( 0.0f, 100.0f ), Rect< SpReal >( 0, 512, 512, 0 ),
		Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), window_texture, true );
	
	shared_ptr< GUI::GuiCheckBox > check = make_shared< GUI::GuiCheckBox >( Math::make_vector( 32.0f, 380.0f ),
		Rect< SpReal >( 0.0f, 16.0f, 16.0f, 0.0f ), Rect< SpReal >( 0.0f, 0.5f, 1.0f, 0.0f ), Rect< SpReal >( 0.5f, 1.0f, 1.0f, 0.0f ),
		check_boxTexture,check_boxTexture,true,true );
	check->Show();
	shared_ptr< GUI::GuiText > guiCheckText = make_shared< GUI::GuiText >( Math::make_vector<SpReal>(50.0f, 380.0f),gfxDriver,
		Rgba(1.0f,1.0f,1.0f),15,m_arialN,L"Check box",true );
	guiCheckText->AllowFocus( false );
	guiCheckText->Show();
	
	shared_ptr< GUI::GuiSlider > guiSlider( new GUI::GuiSlider( Math::make_vector<SpReal>(32.0f, 430.0f ), 
		Rect<SpReal>( 0.0f, 256.0f, 16.0f, 0.0f ), Rect<SpReal>(0.0f,1.0f,1.0f,0.0f ), Rect<SpReal>(0.0f,1.0f,1.0f,0.0f),16, slider_bgTexture,
		slider_texture, GUI::GuiSliderDir::HorizontalDir(), true, true ) );
	guiSlider->Show();
	guiSlider->SetRange( 0, 1000 );
	guiSlider->ConnectHandler( GUI::data_changed, bind( &App::SliderChanged, this, _1, _2, _3 ) );

	shared_ptr< GUI::GuiEditBox > editboxSlider = make_shared< GUI::GuiEditBox >( Math::make_vector( 300.0f,430.0f ), gfxDriver, 
		Rgba( 1.0f, 1.0f, 1.0f ), Rgba(), m_arialN, 6, L"0" );
	editboxSlider->Show();

	m_sliderEditId = editboxSlider->GetID();

	m_window->AddChild( editboxSlider );
	m_window->AddChild( guiSlider );
	m_window->AddChild( guiCheckText );
	m_window->AddChild( check );
	m_window->AddChild( button );
	m_window->AddChild( m_button );
	m_window->AddChild( editbox );
	m_window->AddChild( editbox1 );
	m_window->AddChild( guiText );

	m_button->ConnectHandler( GUI::button_Press, bind( &App::ButtonPress, this, _1, _2, _3 ) );
	engine->GetGuiManager()->AddElement( m_window );

	
	m_window->Show();

	gfxDriver->SetState( RenderState::Depth_Test( RenderState::RS_FALSE ) );
	gfxDriver->SetState( RenderState::Texture( RenderState::RS_TRUE ) );
	gfxDriver->SetState( RenderState::Cull_Face( RenderState::RS_FALSE ) );

	if( engine->CreateSpriteLayers( 1 ) )
	{
		engine->AddSprite( m_sprite.get(), 0 );
		engine->AddSprite( m_sprite_alpha.get(), 0 );
		engine->EnableSpriteLayer( 0, false );
	}

	m_keyDownSubscriber = make_shared< EventSubscriber >( Event( Event::EVENT_ANY, Catagory_KeyBoard::value ) );
	m_keyDownSubscriber->AddCallback( boost::bind( &App::KeyDown, this, _1, _2 ) );
	engine->GetEventPublisher()->AddSubscriber( m_keyDownSubscriber );

	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearColor ), Rgba( 0.5f, 0.5f , 0.5f ) );
	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearColorBuffer ), true );
	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearDepthBuffer ), false );
	engine->SetAttribute( EngineAttribute( EngineAttribute::EnableThreading ), false );

	
	m_camera = new Camera( Math::SpVector3r( 0.0f, 0.0f, 0.0f ) );
	m_camera->SetProjection( Projection::CreateOrtho2D( 0.0f , 0.0f , 1024.0f , 768.0f ) );
	engine->SetCamera( m_camera );


	return true;
}

void App::SliderChanged( boost::int32_t eventId, Spiral::GUI::GuiWindow* window, const boost::any& data )
{
	GUI::GuiSlider* slider = static_cast<GUI::GuiSlider*>( window );
	GUI::GuiEditBox* edit = static_cast<GUI::GuiEditBox*>( m_window->GetChild( m_sliderEditId ) );
	boost::int32_t sliderPos = slider->GetSliderPos();

	SpString str = lexical_cast<SpString>( sliderPos );
	
	UpdateQueue_handle queue;
	queue->Add( bind( &GUI::GuiEditBox::SetText, edit, str ) );
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
	m_arialN.reset();

	return true;
}

SP_DECL_APPLICATION( SimpleApp::App )

App::App():
m_sprite(),
m_sprite_alpha(),
m_keyDownSubscriber(),
m_engine(),
m_arialN(),
m_button(),
m_window(),
m_camera(NULL),
m_sliderEditId(0)
{

}

void App::KeyDown( const Event& event, const any& data )
{
	if( event.IsCat( Catagory_KeyBoard_KeyDown::value ))
	{
		int32_t key = any_cast<int32_t>(data);
		if( key == 27 )
		{
			shared_ptr< Spiral::Engine > theEngine = m_engine.lock();
			// close the app
			if( theEngine )
			{
				theEngine->GetEventPublisher()->Publish( Event( event_AppStatus_shutdown, Catagory_App_Status::value ), any() );
			}
		}
	}
}

void App::ButtonPress( boost::int32_t eventId, Spiral::GUI::GuiWindow* window, const boost::any& data )
{
	LOG_D( "^yApp: ^wButton %1% Pressed!\n", window->GetID() );
}

