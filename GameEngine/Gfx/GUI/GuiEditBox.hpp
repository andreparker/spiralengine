#ifndef GUI_EDIT_BOX_HPP
#define GUI_EDIT_BOX_HPP

#include <string>
#include <stack>

#include "GuiWindow.hpp"
#include "../Color.hpp"
#include "../../Core/Sp_DataTypes.hpp"

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
		   @param     const Math::Vector2f & position
		   @param     const boost::shared_ptr<GfxDriver> & gfxDriver
		   @param     const Rgba & backColor	- backColor of the background of the text window
		   @param     const boost::shared_ptr<Font> & font	- font to render with
		   @param     boost::uint32_t maxCharLen	- max chars allowed in the box
		   @param     const std::string & defText	- defualt text in the box
		*/
		GuiEditBox( const Math::Vector2f& position, const boost::shared_ptr<GfxDriver>& gfxDriver,const Rgba& backColor,
					const Rgba& forColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const wString& defText );

		GuiEditBox();

		/*!
		   @function  GetText
		   @brief     gets the contents of the text box
		   @return    const std::string&
		*/
		const wString& GetText()const;

		/*!
		   @function  SetText
		   @brief     sets the contents of the text box
		   @return    void
		   @param     const std::string & text
		*/
		void SetText( const wString& text );

		void SetFont( const boost::shared_ptr< Font >& font, GfxDriver* gfxDriver );
		void SetMaxCharLen( boost::uint32_t maxCharLen );
		void SetFontColor( const Rgba& color );
		void SetMultiLine( bool isMulti );

		void SetBkGroundColor( const Rgba& color );
	private:

		void DrawString( const wString& str );
		void OnChar( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnDataChanged( boost::int32_t eventId, GuiWindow* window, const boost::any& data );

		void Resize( boost::int32_t width, boost::int32_t height, const boost::shared_ptr< GfxDriver >& gfxDriver );
		void RefreshBackGround();
	private:

		boost::shared_ptr<GuiText> m_textBox;
		Rgba       m_backColor;
		boost::uint32_t m_maxCharLen;
	};
}
}
#endif