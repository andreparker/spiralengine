#include "logger.hpp"
#include "iterator_utility.ipp"
#include "engineassert.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>

using namespace gameEngine;

LogRouter::LogRouter()
{
}

LogRouter::~LogRouter()
{
}

namespace
{
	int32 _flags = 0;
	int8* _buffer = NULL;

	void log_flags( int32 flags )
	{
		_flags = flags;
	}

	void log_buffer( const int8* buffer )
	{
		_buffer = const_cast< int8* >( buffer );
	}

	void log_out( LogModule* logger )
	{
		if( logger->canLog( _flags ) )
			logger->log( _buffer );
	}
}

void LogRouter::addLogger( gameEngine::LogModule *logger)
{
	m_loggers.insertTail( logger );
}

void LogRouter::format( int8* buffer, const int8* msg, ... )
{
	va_list args;
	int count = 0;

	ASSERT( msg != NULL );

	va_start( args, msg );
		count = vsprintf( &buffer[0], msg, args );
	va_end( args );
}

void LogRouter::logNoArg( int32 flags, const int8* msg )
{
	log_flags( flags );
	log_buffer( msg );

	common::for_each( m_loggers.head(), m_loggers.end(), log_out );
}

void LogRouter::log( gameEngine::int32 flags, const gameEngine::int8 *msg, ...)
{
	va_list args;
	int8 buffer[128];
	int count = 0;

	ASSERT( msg != NULL );

	va_start( args, msg );
		count = vsprintf( &buffer[0], msg, args );
	va_end( args );


	// test for errors
	ASSERT( count != 0 );

	log_flags( flags );
	log_buffer( &buffer[0] );

	common::for_each( m_loggers.head(), m_loggers.end(), log_out );
}