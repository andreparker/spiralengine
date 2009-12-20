#include "GuiSpriteEditor.hpp"
#include "../Core/Rect.hpp"
#include "../Core/Sp_DataTypes.hpp"
#include "../Math/Math.hpp"

#include "../Gfx/gui/GuiButton.hpp"
#include "../Gfx/gui/GuiEditBox.hpp"
#include "../Gfx/gui/GuiCheckBox.hpp"

#include "../Core/Engine.hpp"

using namespace Editor;
using namespace Spiral;

bool GuiSpriteEditor::Initialize( const boost::shared_ptr< Spiral::Engine >& engine )
{
	bool isInitialized = true;

	return isInitialized;
}

GuiSpriteEditor::GuiSpriteEditor():
m_parentWindow(),
m_editImageFileName(),
m_checkAlphaBlend(),
m_checkAdditiveAlphaBlend(),
m_editWidth(),m_editHeight(),m_editAngle(),
m_editRotationOffsetX(),m_editRotationOffsetY(),
m_saveData(),m_loadData()
{

}