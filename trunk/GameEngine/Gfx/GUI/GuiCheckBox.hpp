#ifndef GUI_CHECK_BOX_HPP
#define GUI_CHECK_BOX_HPP

#include "GuiButton.hpp"

namespace Spiral
{

namespace GUI
{
	class GuiCheckBox : public GuiButton
	{
	public:
		GuiCheckBox( const Math::SpVector2r& position, const Rect< SpReal >& rect, 
			const Rect< SpReal >& bgTextCoords,const Rect< SpReal >& ckTextCoords,
			const boost::shared_ptr< Texture >& bgTexture,const boost::shared_ptr< Texture >& ckTexture
			, bool bBgAlpha, bool bCkAlpha );

		virtual ~GuiCheckBox();

		bool IsChecked()const
		{
			return m_isChecked;
		}

		void SetCheck( bool isChecked );
	private:

		void CheckPressed( boost::int32_t eventId, GuiWindow* window, const boost::any& data );

	private:
		bool m_isChecked;
		boost::shared_ptr<GuiWindow> m_ckWindow;
	};
}
}

#endif