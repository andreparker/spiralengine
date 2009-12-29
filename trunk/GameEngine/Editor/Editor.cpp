#include "Editor.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace Editor;
using namespace Spiral;
using namespace boost;

App::App():
m_engine(),m_camera(),m_spriteEditor(),m_editorBg(),
m_bgTexture(),m_bgTextCoords(),m_bgRect()
{
}

bool App::DoInit( boost::int32_t argc, std::list< shared_array< char > >& argList, shared_ptr< Engine >& engine )
{
	bool isInitialize = true;
	m_engine = engine;  // get a weak ptr to the engine

	m_camera.reset( new Camera() );
	m_camera->SetProjection( Projection::CreateOrtho2D( 0.0f, 0.0f, 1024.0f, 1024.0f ) );
	engine->SetCamera( m_camera.get() );

	shared_ptr< GfxDriver > gfxDriver = engine->GetGfxDriver();
	gfxDriver->SetState( RenderState::Depth_Test( RenderState::RS_FALSE ) );
	gfxDriver->SetState( RenderState::Texture( RenderState::RS_TRUE ) );
	gfxDriver->SetState( RenderState::Cull_Face( RenderState::RS_FALSE ) );
	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearColor ), Rgba( 0.5f, 0.5f , 0.5f ) );
	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearColorBuffer ), false );
	engine->SetAttribute( EngineAttribute( EngineAttribute::ClearDepthBuffer ), false );
	engine->SetAttribute( EngineAttribute( EngineAttribute::EnableThreading ), false );

	shared_ptr< Font > arialn = engine->LoadFont( "c:/windows/fonts/arialn.ttf", "arial_n", 12, 14 );
	m_bgTexture = engine->LoadTexture( "Data/GUI/edit_background.png", "edit_bg_texture" );

	shared_ptr< GUI::GuiManager > guiManager = engine->GetGuiManager();

	m_bgTextCoords.Set( 0.0f, 1024.0f/m_bgTexture->GetWidth(),0.0f, 1024.0f/m_bgTexture->GetHeight() );
	m_bgRect.Set(0.0f,1024.0f,0.0f,1024.0f);
	m_editorBg = make_shared< GUI::GuiWindow >( Math::make_vector(0.0f,0.0f),m_bgRect,m_bgTextCoords, shared_ptr<Texture>(), false );
	m_editorBg->Show();

	CreateEditorButtons( guiManager, arialn );

	m_spriteEditor = make_shared< GuiSpriteEditor >();
	m_spriteEditor->Initialize( engine );
	m_editorBg->AddChild( m_spriteEditor->GetMainWindow() );

	guiManager->AddElement( m_editorBg );

	return isInitialize;
}


bool App::DoRun( Spiral::SpReal ticks, boost::shared_ptr< Spiral::Engine >& engine )
{
	engine->GetGfxDriver()->Bind( m_bgTexture );
	engine->GetGfxDriver()->Draw( Math::make_vector(0.0f,0.0f), m_bgRect, m_bgTextCoords );
	
	return true;
}

bool App::DoUnInit()
{
	m_spriteEditor.reset();
	m_editorBg.reset();
	m_bgTexture.reset();

	return true;
}

void App::CreateEditorButtons( const shared_ptr< GUI::GuiManager >& guiManager,const boost::shared_ptr<Font>& font  )
{
	shared_ptr< GUI::GuiButton > spriteEditorButton = guiManager->Make_DefButton( 10.0f, 8.0f , 64.0f, 32.0f );
	shared_ptr< GUI::GuiButton > particleEditorButton = guiManager->Make_DefButton( 94.0f, 8.0f , 64.0f, 32.0f );
	shared_ptr< GUI::GuiButton > gameEditorButton = guiManager->Make_DefButton( 178.0f, 8.0f , 64.0f, 32.0f );
	shared_ptr< GUI::GuiButton > visualEditorButton = guiManager->Make_DefButton( 262.0f, 8.0f , 64.0f, 32.0f );

	shared_ptr< GUI::GuiText > spriteEdText = guiManager->Make_DefTextBox( 4.0f, 8.0f, Rgba( 1.0f, 1.0f, 1.0f ), font, 15, L"Sprite Edit" );
	shared_ptr< GUI::GuiText > particleEdText = guiManager->Make_DefTextBox( 4.0f, 8.0f, Rgba( 1.0f, 1.0f, 1.0f ), font, 15, L"Particle Edit" );
	shared_ptr< GUI::GuiText > gameEdText = guiManager->Make_DefTextBox( 4.0f, 8.0f, Rgba( 1.0f, 1.0f, 1.0f ), font, 15, L"Game Edit" );
	shared_ptr< GUI::GuiText > visualEdText = guiManager->Make_DefTextBox( 4.0f, 8.0f, Rgba( 1.0f, 1.0f, 1.0f ), font, 15, L"Visual Edit" );

	spriteEdText->AllowFocus(false);
	spriteEdText->Show();
	particleEdText->AllowFocus(false);
	particleEdText->Show();
	gameEdText->AllowFocus(false);
	gameEdText->Show();
	visualEdText->AllowFocus(false);
	visualEdText->Show();

	spriteEditorButton->AddChild( spriteEdText );
	particleEditorButton->AddChild( particleEdText );
	gameEditorButton->AddChild( gameEdText );
	visualEditorButton->AddChild( visualEdText );

	spriteEditorButton->Show();
	particleEditorButton->Show();
	gameEditorButton->Show();
	visualEditorButton->Show();

	m_editorBg->AddChild( spriteEditorButton );
	m_editorBg->AddChild( particleEditorButton );
	m_editorBg->AddChild( gameEditorButton );
	m_editorBg->AddChild( visualEditorButton );
}