#ifndef PLATFORM_WINDOWS_HPP
#define PLATFORM_WINDOWS_HPP

#include "Win32Util.h"
#include "../core/types.hpp"
#include "../core/objectmanager.hpp"
#include "../core/app.hpp"
#include <windows.h>

class AppWindow : public Window32
{
	public:
		AppWindow( gameEngine::frameWork::coreApp* app );
		~AppWindow();

		virtual void OnDestroy();
	protected:
};



#endif