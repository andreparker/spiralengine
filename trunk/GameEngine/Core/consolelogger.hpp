#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include "logger.hpp"
#include "singleton.ipp"

namespace gameEngine
{
	class ConsoleLog : public LogModule,
		public singleton< ConsoleLog >
	{
			DECL_SINGLETON( ConsoleLog )
		protected:
			ConsoleLog();

			int32 m_flags;
		public:
			
			void log(const int8* msg);
			bool canLog( int32 flags )const;
	};
}

#endif