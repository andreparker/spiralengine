#ifndef GUI_TEXT_HPP
#define GUI_TEXT_HPP

#include <stack>
#include "GuiWindow.hpp"
#include "../Color.hpp"

namespace Spiral
{

	class SurfaceUP;
	class GfxDriver;
	class Font;
namespace GUI
{

	class GuiText : public GuiWindow
	{
	public:
		virtual ~GuiText(){}
		GuiText( const Math::SpVector2r& position, const boost::shared_ptr<GfxDriver>& gfxDriver,
			const Rgba& textColor, boost::uint32_t maxCharLen, const boost::shared_ptr<Font>& font, const std::string& text );

		void SetCharPos( boost::uint32_t posX )
		{
			m_charPos = posX;
		}

		boost::uint32_t GetCharPos()const
		{
			return m_charPos;
		}

		const std::string& GetText()const
		{
			return m_text;
		}

		/*!
		   @function  SetText
		   @brief     sets the text
		   @return    void
		   @param     const std::string & text
		*/
		void SetText( const std::string& text );

	private:

		void DrawString( const std::string& text );
		void DrawChar( char c );
	private:
		std::string m_text;
		boost::uint32_t m_charPos;
		boost::shared_ptr<Font> m_font;
		boost::shared_ptr<SurfaceUP> m_editSurface;
		const boost::uint32_t m_maxCharLen;
		Rgba m_fontColor;
		std::stack<boost::uint32_t> m_oldCursorPos;

	};
}
}
#endif