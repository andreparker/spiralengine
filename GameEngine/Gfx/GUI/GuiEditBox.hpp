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
					const Rgba& forColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const SpString& defText );

		/*!
		   @function  GetText
		   @brief     gets the contents of the text box
		   @return    const std::string&
		*/
		const SpString& GetText()const;

		/*!
		   @function  SetText
		   @brief     sets the contents of the text box
		   @return    void
		   @param     const std::string & text
		*/
		void SetText( const SpString& text );
	private:

		void DrawString( const SpString& str );
		void OnChar( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
		void OnDataChanged( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
	private:

		boost::shared_ptr<GuiText> m_textBox;
	};
}
}
#endif