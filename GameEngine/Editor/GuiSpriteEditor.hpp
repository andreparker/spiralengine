#ifndef GUI_SPRITE_EDITOR_HPP
#define GUI_SPRITE_EDITOR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "../Gfx/gui/GuiWindow.hpp"
#include "../Gfx/gui/GuiButtonFwd.hpp"
#include "../Gfx/gui/GuiEditBoxFwd.hpp"
#include "../Gfx/gui/GuiCheckBoxFwd.hpp"

#include "../Core/EngineFwd.hpp"

namespace Editor
{
	class GuiSpriteEditor :  boost::noncopyable
	{
	public:
		GuiSpriteEditor();

		bool Initialize( const boost::shared_ptr< Spiral::Engine >& engine );
	private:
		boost::shared_ptr<Spiral::GUI::GuiWindow> m_parentWindow;
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editImageName;
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editImageFileName;
		boost::shared_ptr<Spiral::GUI::GuiCheckBox> m_checkAlphaBlend;
		boost::shared_ptr<Spiral::GUI::GuiCheckBox> m_checkAdditiveAlphaBlend;
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editWidth,m_editHeight,m_editAngle,
											m_editRotationOffsetX,m_editRotationOffsetY;

		boost::shared_ptr<Spiral::GUI::GuiButton> m_saveData,m_loadData;

	};
}
#endif