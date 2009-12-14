#ifndef GUI_TEXT_HPP
#define GUI_TEXT_HPP

#include <stack>
#include "GuiWindow.hpp"
#include "../Color.hpp"
#include "../../Core/Sp_DataTypes.hpp"

namespace Spiral
{

	class Surface;
	class GfxDriver;
	class Font;
namespace GUI
{

	class GuiText : public GuiWindow
	{
	public:
		virtual ~GuiText(){}
		GuiText( const Math::SpVector2r& position, const boost::shared_ptr<GfxDriver>& gfxDriver,
			const Rgba& textColor, boost::uint32_t maxCharLen, const boost::shared_ptr<Font>& font, const SpString& text, bool multiLine = false );

		void SetCharPos( boost::uint32_t posX )
		{
			m_charPos = posX;
		}

		boost::uint32_t GetCharPos()const
		{
			return m_charPos;
		}

		const SpString& GetText()const
		{
			return m_text;
		}

		bool IsMultiLine()const
		{
			return m_multiline;
		}

		/*!
		   @function  SetText
		   @brief     sets the text
		   @return    void
		   @param     const std::string & text
		*/
		void SetText( const SpString& text );
		void DrawChar( SpChar c );

		void EraseEnd();
		void ClearBox();
	private:

		void DrawString( const SpString& text );
		
	private:
		SpString m_text;
		boost::uint32_t m_charPos;
		boost::shared_ptr<Font> m_font;
		boost::shared_ptr<Surface> m_editSurface;
		const boost::uint32_t m_maxCharLen;
		Rgba m_fontColor;
		bool m_multiline;

	};
}
}
#endif