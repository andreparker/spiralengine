#ifndef GUI_COMBO_BOX_HPP
#define GUI_COMBO_BOX_HPP

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "GuiWindow.hpp"
#include "../../Core/Sp_DataTypes.hpp"

namespace Spiral
{

	class Font;
	class GfxDriver;
	class GuiList;
	class GuiButton;
	class Texture;
namespace GUI
{

	class GuiComboBox : public GuiWindow
	{
	public:
		GuiComboBox( const Math::Vector2f& position, const Rect<SpReal>& texCoords,
			const boost::shared_ptr<Texture>& texture, const boost::shared_ptr<GuiButton>& button,
			const boost::shared_ptr<Font>& font, bool alpha );

		boost::int32_t GetSelection()const;
		std::string& GetItemText( boost::int32_t index )const;
		boost::int32_t GetItemInt( boost::int32_t index )const;
		SpReal GetItemReal( boost::int32_t index )const;

		bool AddItemText( const SpString& item );
		bool AddItemInt( boost::int32_t item );
		bool AddItemReal( SpReal item );

		// button that enables the list box
		void OnButtonClick( boost::int32_t eventId, GuiWindow* window, const boost::any& data );
	private:
		boost::scoped_ptr< GuiList > m_list;		/// list box
		boost::shared_ptr< GuiButton > m_button;		///< button to enable the list box
		bool m_listEnabled;				///< specifies if the list box is shown/enabled
	};
}

}
#endif