#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "TypeUtils.hpp"
#include <boost/cstdint.hpp>

namespace Spiral
{

	// byte0
	enum
	{
		cat_Input = 1,
		cat_Sys
	};

	// byte1
	enum
	{
		cat_Mouse = 1,
		cat_Keyboard,
		cat_App
	};

	// byte2
	enum
	{
		cat_Mouse_Down = 1,
		cat_Mouse_Up,
		cat_Mouse_Move,
		cat_Keyboard_KeyDown,
		cat_Keyboard_KeyUp,
		cat_Status
	};

	// events
	enum
	{
		event_keyboard_key = 1,
		event_mouse,
		event_AppStatus_shutdown
	};

	typedef MakeLongValue< cat_Input, 0, 0, 0 >                               Catagory_Input;
	typedef MakeLongValue< cat_Input, cat_Keyboard, cat_Keyboard_KeyDown, 0 > Catagory_KeyBoard_KeyDown;
	typedef MakeLongValue< cat_Input, cat_Keyboard, cat_Keyboard_KeyUp, 0 >   Catagory_KeyBoard_KeyUp;
	typedef MakeLongValue< cat_Input, cat_Keyboard, 0, 0 >                    Catagory_KeyBoard;

	typedef MakeLongValue< cat_Input, cat_Mouse, cat_Mouse_Down, 0 >          Catagory_Mouse_MouseDown;
	typedef MakeLongValue< cat_Input, cat_Mouse, cat_Mouse_Up, 0 >            Catagory_Mouse_Up;
	typedef MakeLongValue< cat_Input, cat_Mouse, cat_Mouse_Move, 0 >          Catagory_Mouse_Move;
	typedef MakeLongValue< cat_Input, cat_Mouse, 0 , 0 >                      Catagory_Mouse;

	typedef MakeLongValue< cat_Sys, cat_App, cat_Status, 0 >                  Catagory_App_Status;

}

#endif