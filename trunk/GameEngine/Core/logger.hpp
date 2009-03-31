#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "types.hpp"
#include "linkedlist.ipp"
#include "singleton.ipp"
#include "engineassert.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace gameEngine
{
	class LogModule
	{
		public:
			virtual bool canLog( int32 flags )const { return false; }
			virtual void log( const int8* msg ){}
	};

	class LogRouter : 
		public singleton< LogRouter >
	{
		LogRouter();
		~LogRouter();
		DECL_SINGLETON( LogRouter )

		public:
			void addLogger( LogModule* logger );
			void log( int32 flags, const int8* msg, ... );
			void logNoArg( int32 flags, const int8* msg );
			void format( int8* buffer, const int8* msg, ... );

		protected:
			common::linkedList< LogModule* > m_loggers;

	};

	///< flags
	enum{

		logFlag_error = bit< 0 >::value,  ///< handles error results
		logFlag_results = bit< 1 >::value, ///< handles results no matter if its an error or not
		logFlag_platform = bit< 2 >::value, ///< platform spacific things only
		logFlag_system_init = bit< 3 >::value, ///< logs out engine specific initializtion
		logFlag_file = bit< 4 >::value ///< handles file logging
	};
}

template< gameEngine::int32 bits >
void log( const gameEngine::int8* msg, ... )
{
	gameEngine::int8 buffer[128];
	va_list args;
	int count = 0;

	ASSERT( msg != NULL );

	va_start( args, msg );
		count = vsprintf( &buffer[0], msg, args );
	va_end( args );

	gameEngine::LogRouter::instance().logNoArg( bits, buffer );
}


#define LOG_I log< logFlag_system_init >
#define LOG_E log< logFlag_error >

#endif