#ifndef WINDOWS_LOGGER_HPP
#define WINDOWS_LOGGER_HPP

#include "../../Core/Log.hpp"
#include <Windows.h>

namespace Spiral
{
	class WindowsLogger : public LogModule
	{
	public:
		WindowsLogger( HWND mainWindow );

	private:
		virtual bool canLog( const LogFlags& flags )const 
		{
			return true; 
		}

		virtual void log( const std::string& msg )const;
	private:
		HWND m_recieverWindow;
		HWND m_senderWindow;
	};
}
#endif