#ifndef GUI_EDIT_BOX_HPP
#define GUI_EDIT_BOX_HPP

#include <string>
#include <stack>

#include "GuiWindow.hpp"
#include "../Color.hpp"

namespace Spiral
{

	class Font;
	class GfxDriver;
	class SurfaceUP;
namespace GUI
{
	class GuiText;
	class GuiEditBox : public GuiWindow
	{
	public:
		/*!
		   @function  GuiEditBox
		   @brief     creates a edit text box
		   @return    
		   @param     const Math::SpVector2r & position
		   @param     const boost::shared_ptr<GfxDriver> & gfxDriver
		   @param     const Rgba & backColor	- backColor of the background of the text window
		   @param     const boost::shared_ptr<Font> & font	- font to render with
		   @param     boost::uint32_t maxCharLen	- max chars allowed in the box
		   @param     const std::string & defText	- defualt text in the box
		*/
		GuiEditBox( const Math::SpVector2r& position, const boost::shared_ptr<GfxDriver>& gfxDriver,const Rgba& backColor,
					const Rgba& forColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const std::string& defText );

		/*!
		   @function  GetText
		   @brief     gets the contents of the text box
		   @return    const std::string&
		*/
		const std::string& GetText()const;

		/*!
		   @function  SetText
		   @brief     sets the contents of the text box
		   @return    void
		   @param     const std::string & text
		*/
		void SetText( const std::string& text );
	private:

		void DrawString( const std::string& str );
	private:

		boost::shared_ptr<GuiText> m_textBox;
// 		boost::uint32_t m_cursorPos;
// 		std::string m_text;
// 		boost::shared_ptr<Font> m_font;
// 		boost::shared_ptr<Texture> m_editTexture;		// texture that the text will be rendered to
// 		const boost::uint32_t m_maxCharLen;				// max characters in the edit box
// 		std::stack<boost::uint32_t> m_oldCursorPos;		// last char positon
// 		boost::shared_ptr<SurfaceUP> m_editSurface;     // surface to allow text to be rendered
// 		Rgba m_forgroundColor;							// text color
	};
}
}
#endif