#include "GuiSpriteEditor.hpp"
#include "../Core/Rect.hpp"
#include "../Core/Sp_DataTypes.hpp"
#include "../Math/Math.hpp"

#include "../Gfx/Color.hpp"

#include "../Gfx/gui/Gui.hpp"
#include "../Gfx/gui/GuiButton.hpp"
#include "../Gfx/gui/GuiEditBox.hpp"
#include "../Gfx/gui/GuiCheckBox.hpp"
#include "../Gfx/gui/GuiFrame.hpp"
#include "../Gfx/gui/GuiText.hpp"

#include "../Core/Engine.hpp"
#include "../Gfx/Font.hpp"

using namespace Editor;
using namespace Spiral;
using namespace boost;

bool GuiSpriteEditor::Initialize( const boost::shared_ptr< Spiral::Engine >& engine )
{
	bool isInitialized = true;
	shared_ptr< GUI::GuiManager > guiManager = engine->GetGuiManager();
	SpReal nextPosY = 40.0f, nextPosX = 100.0f;
	const Rgba kEditColor( 0.0f, 0.0f, 0.0f );
	const Rgba kEditBgColor( 1.0f, 1.0f, 1.0f );
	const Rgba kFontColor( 1.0f, 1.0f, 1.0f );
	const uint32_t kMaxEditLen = 32;

	shared_ptr< Font > arialn = engine->LoadFont( "c:/windows/fonts/arialn.ttf", "arial_n", 12, 14 );
	SpReal fontHeight = arialn->GetCharHeight() + 16.0f;
	SpReal elementSpace = 2.0f;

	m_parentWindow = guiManager->Make_DefFrame( 512.0f, 512.0f, 350.0f, 512.0f );
	m_editImageName = guiManager->Make_DefEditBox( nextPosX, nextPosY, kEditBgColor, kEditColor, arialn, kMaxEditLen,L"" );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen, L"Image Name:" );

	m_parentWindow->AddChild( m_editImageName );
	nextPosY += fontHeight + elementSpace;

	m_editImageFileName = guiManager->Make_DefEditBox( nextPosX, nextPosY, kEditBgColor, kEditColor, arialn, kMaxEditLen,L"" );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen,  L"Image File Name:" );
	
	m_parentWindow->AddChild( m_editImageFileName );
	nextPosY += fontHeight + elementSpace;

	m_editWidth = guiManager->Make_DefEditBox( nextPosX, nextPosY, kEditBgColor, kEditColor, arialn, kMaxEditLen,L"32" );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen,  L"Width:" );

	m_parentWindow->AddChild( m_editWidth );
	nextPosY += fontHeight + elementSpace;

	m_editHeight = guiManager->Make_DefEditBox( nextPosX, nextPosY, kEditBgColor, kEditColor, arialn, kMaxEditLen,L"32" );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen,  L"Height:" );

	m_parentWindow->AddChild( m_editHeight );
	nextPosY += fontHeight + elementSpace;

	m_editRotationOffsetX = guiManager->Make_DefEditBox( nextPosX, nextPosY, kEditBgColor, kEditColor, arialn, kMaxEditLen,L"0" );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen,  L"RotationOffsetX:" );

	m_parentWindow->AddChild( m_editRotationOffsetX );
	nextPosY += fontHeight + elementSpace;

	m_editRotationOffsetY = guiManager->Make_DefEditBox( nextPosX, nextPosY, kEditBgColor, kEditColor, arialn, kMaxEditLen,L"0" );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen,  L"RotationOffsetY:" );

	m_parentWindow->AddChild( m_editRotationOffsetY );
	nextPosY += fontHeight + elementSpace;

	m_editAngle = guiManager->Make_DefEditBox( nextPosX, nextPosY, kEditBgColor, kEditColor, arialn, kMaxEditLen,L"0" );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen,  L"Angle:" );

	m_parentWindow->AddChild( m_editAngle );
	nextPosY += fontHeight + elementSpace;

	m_checkAlphaBlend = guiManager->Make_DefCheckBox( nextPosX, nextPosY, 16.0f, 16.0f );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen,  L"Alpha:" );

	m_parentWindow->AddChild( m_checkAlphaBlend );
	nextPosY += fontHeight + elementSpace;

	m_checkAdditiveAlphaBlend = guiManager->Make_DefCheckBox( nextPosX, nextPosY, 16.0f, 16.0f );
	AddTextBox(guiManager, nextPosY, kFontColor, arialn, kMaxEditLen,  L"Additive:" );

	m_parentWindow->AddChild( m_checkAdditiveAlphaBlend );
	nextPosY += 16.0f + elementSpace; nextPosX = 200.0f;

	m_saveData = guiManager->Make_DefButton( nextPosX, 438.0f , 64.0f, 32.0f );
	AddText(guiManager, kFontColor, arialn, m_saveData, L"Save" );


	m_parentWindow->AddChild( m_saveData );
	nextPosX += 64.0f + 10.0f;

    m_loadData = guiManager->Make_DefButton( nextPosX, 438.0f , 64.0f, 32.0f );
	AddText(guiManager, kFontColor, arialn, m_loadData, L"Load" );

	m_parentWindow->AddChild( m_loadData );

	m_parentWindow->Show();
	m_editImageName->Show();
	m_editImageFileName->Show();
	m_checkAlphaBlend->Show();
	m_checkAdditiveAlphaBlend->Show();
	m_editWidth->Show();
	m_editHeight->Show();
	m_editAngle->Show();
	m_editRotationOffsetX->Show();
	m_editRotationOffsetY->Show();
	m_saveData->Show();
	m_loadData->Show();
	
	guiManager->AddElement( m_parentWindow );

	return isInitialized;
}

GuiSpriteEditor::GuiSpriteEditor():
m_parentWindow(),
m_editImageName(),
m_editImageFileName(),
m_checkAlphaBlend(),
m_checkAdditiveAlphaBlend(),
m_editWidth(),m_editHeight(),m_editAngle(),
m_editRotationOffsetX(),m_editRotationOffsetY(),
m_saveData(),m_loadData()
{

}

void GuiSpriteEditor::Show( bool show /*= true */ )
{
	m_parentWindow->Show( show );

}

void GuiSpriteEditor::AddTextBox( const shared_ptr< GUI::GuiManager >& guiManager, SpReal nextPosY, const Rgba& kFontColor, 
								 const shared_ptr< Font >& arialn, uint32_t kMaxEditLen, const Spiral::SpString& text  )
{
	shared_ptr< GUI::GuiText > addedText = guiManager->Make_DefTextBox( 10.0f, nextPosY, kFontColor, arialn, kMaxEditLen, text );

	addedText->Show();
	addedText->AllowFocus(false);

	m_parentWindow->AddChild( addedText );

	
}

void GuiSpriteEditor::AddText( const shared_ptr< GUI::GuiManager >& guiManager, const Rgba& kFontColor,const shared_ptr< Font >& arialn,
			                   const shared_ptr<GUI::GuiButton>& button, const SpString& text )
{
	shared_ptr< GUI::GuiText > saveText = guiManager->Make_DefTextBox( 10.0f, 8.0f, kFontColor, arialn, 8, text );
	button->AddChild( saveText );
	saveText->Show();
	saveText->AllowFocus(false);
}