#ifndef GUI_SPRITE_EDITOR_HPP
#define GUI_SPRITE_EDITOR_HPP

#include <boost/shared_ptr.hpp>

#include "../Gfx/gui/GuiWindow.hpp"
#include "../Gfx/gui/GuiButton.hpp"
#include "../Gfx/gui/GuiEditBox.hpp"
#include "../Gfx/gui/GuiCheckBox.hpp"

namespace Editor
{
	class GuiSpriteEditor : public Spiral::GUI::GuiWindow
	{
	public:
		GuiSpriteEditor();

	private:
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editImageName;
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editImageFileName;
		boost::shared_ptr<Spiral::GUI::GuiCheckBox> m_checkAlphaBlend;
		boost::shared_ptr<Spiral::GUI::GuiCheckBox> m_checkAdditiveAlphaBlend;
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editWidth,m_editHeight,m_editAngle,
											m_editRotationOffsetX,m_editRotationOffsetY;

	};
}
#endif