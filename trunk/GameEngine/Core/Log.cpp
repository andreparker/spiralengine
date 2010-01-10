#include "Log.hpp"
#include "../Script/ScriptManager.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace Spiral;
using namespace boost;
using namespace std;

LogRouter::LogRouter()
{
}

LogRouter::~LogRouter()
{
}

void LogRouter::Register( const shared_ptr<ScriptManager>& scriptMgr )
{
	luabind::scope s = luabind::namespace_("Log")
	[
		luabind::def( "Log_I", Spiral::ScriptLog<Int2Bit<logFlag_system_init>::value> ),
		luabind::def( "Log_E", Spiral::ScriptLog<Int2Bit<logFlag_error>::value> )
	];

	scriptMgr->RegisterModule( s );
}

void LogRouter::addLogger( shared_ptr< LogModule >& logger )
{
	m_loggers.push_back( logger );
}

void LogRouter::format( string& buffer, const string& msg, ... )const
{
	va_list args;
	char buff[128];


	va_start( args, msg );
		vsprintf( &buff[0], msg.c_str(), args );
	va_end( args );

	buffer = buff;
}

void LogRouter::logNoArg( const LogFlags& flags, const string& msg )const
{
	BOOST_FOREACH( shared_ptr< LogModule > logger, m_loggers )
	{
		if( logger && logger->CanLog( flags ) )
		{
			logger->Log( msg );
		}
	}

}

void LogRouter::log( const LogFlags& flags, const string& msg, ...)const
{
	va_list args;
	char buffer[128];


	va_start( args, msg );
		vsprintf( &buffer[0], msg.c_str(), args );
	va_end( args );


	BOOST_FOREACH( shared_ptr< LogModule > logger, m_loggers )
	{
		if( logger->CanLog( flags ) )
		{
			logger->Log( msg );
		}
	}
}

void LogModule::Log( const std::string& msg )const
{
	log( msg );
}

bool LogModule::CanLog( const LogFlags& flags )const
{
	return canLog( flags );
}

std::string LogScopeObject::m_space = "";

std::string StripColorCodes( const std::string& logStr )
{
	if( logStr.find('^') != logStr.npos )
	{
		const boost::char_separator<char> delims( "^" );
		boost::tokenizer< boost::char_separator<char> > tokens( logStr, delims );
		boost::tokenizer< boost::char_separator<char> >::iterator itr = tokens.begin();

		std::string final;
		for(; itr != tokens.end();++itr )
		{
			final += std::string( ((*itr).c_str() + 1), (*itr).length()-1 );
		}

		return final;
	}
	
	return logStr;
}

