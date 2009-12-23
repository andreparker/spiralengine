#ifndef GUI_SPRITE_EDITOR_HPP
#define GUI_SPRITE_EDITOR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

#include "../Core/Sp_DataTypes.hpp"
#include "../Gfx/gui/GuiButtonFwd.hpp"
#include "../Gfx/gui/GuiEditBoxFwd.hpp"
#include "../Gfx/gui/GuiCheckBoxFwd.hpp"
#include "../Gfx/gui/GuiFrameFwd.hpp"
#include "../Gfx/gui/GuiFwd.hpp"
#include "../Gfx/ColorFwd.hpp"
#include "../Gfx/FontFwd.hpp"

#include "../Core/EngineFwd.hpp"

namespace Editor
{
	class GuiSpriteEditor :  boost::noncopyable
	{
	public:
		GuiSpriteEditor();

		bool Initialize( const boost::shared_ptr< Spiral::Engine >& engine );
		void Show( bool show = true );

	private:
		boost::shared_ptr<Spiral::GUI::GuiFrame> m_parentWindow;
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editImageName;
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editImageFileName;
		boost::shared_ptr<Spiral::GUI::GuiCheckBox> m_checkAlphaBlend;
		boost::shared_ptr<Spiral::GUI::GuiCheckBox> m_checkAdditiveAlphaBlend;
		boost::shared_ptr<Spiral::GUI::GuiEditBox> m_editWidth,m_editHeight,m_editAngle,
											m_editRotationOffsetX,m_editRotationOffsetY;

		boost::shared_ptr<Spiral::GUI::GuiButton> m_saveData,m_loadData;
	private:
		void AddTextBox( const boost::shared_ptr< Spiral::GUI::GuiManager >& guiManager, Spiral::SpReal nextPosY, 
			const Spiral::Rgba& kFontColor, const boost::shared_ptr< Spiral::Font >& arialn, boost::uint32_t kMaxEditLen,
			const Spiral::SpString& text );

		void AddText( const boost::shared_ptr< Spiral::GUI::GuiManager >& guiManager, const Spiral::Rgba& kFontColor,
			const boost::shared_ptr< Spiral::Font >& arialn,const boost::shared_ptr<Spiral::GUI::GuiButton>& button, const Spiral::SpString& text );

	};
}
#endif