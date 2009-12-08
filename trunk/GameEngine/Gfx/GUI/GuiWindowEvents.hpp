#ifndef GUI_WINDOW_EVENTS_HPP
#define GUI_WINDOW_EVENTS_HPP

#include "../../Core/Sp_DataTypes.hpp"

namespace Spiral
{

namespace GUI
{
	typedef enum
	{
		mouse_down = 1,
		mouse_up,
		mouse_hover,
		focus_gained,
		focus_lost,
		button_Press,
		char_input
	}GuiWindowEvent;


	struct mouse_position
	{
		mouse_position( SpReal x_, SpReal y_ ):
		x(x_),y(y_){}
		SpReal x,y;
	};
}
}

#endif