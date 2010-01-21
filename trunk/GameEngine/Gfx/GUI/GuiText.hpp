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
		GuiText( const Math::Vector2f& position, const boost::shared_ptr<GfxDriver>& gfxDriver,
			const Rgba& textColor, boost::uint32_t maxCharLen, const boost::shared_ptr<Font>& font, const wString& text, bool multiLine = false );

		GuiText();

		void SetCharPos( boost::uint32_t posX )
		{
			m_charPos = posX;
		}

		boost::uint32_t GetCharPos()const
		{
			return m_charPos;
		}

		const wString& GetText()const
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
		void SetText( const wString& text );
		void DrawChar( wChar c );

		void EraseEnd();
		void ClearBox();

		void SetFont( const boost::shared_ptr< Font >& font, GfxDriver* gfxDriver );
		void SetMaxCharLen( boost::uint32_t maxCharLen );
		void SetFontColor( const Rgba& color );
		void SetMultiLine( bool isMulti )
		{
			m_multiline = isMulti;
			UpdateBox();
		}
		
		void Refresh();
	private:

		void DrawString( const wString& text );
		void ResizeBox( boost::int32_t width, boost::int32_t height );
		
		void UpdateBox();
		
	private:
		wString m_text;
		boost::uint32_t m_charPos;
		boost::shared_ptr<Font> m_font;
		boost::shared_ptr<Surface> m_editSurface;
		boost::uint32_t m_maxCharLen;
		Rgba m_fontColor;
		bool m_multiline;
		GfxDriver* m_gfxDriver;

	};
}
}
#endif