#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "singleton.ipp"
#include "TypeUtils.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/format.hpp>

namespace Spiral
{
	typedef std::bitset<16> LogFlags;

	class LogModule
	{
	public:
		bool CanLog( const LogFlags& flags )const;
		void Log( const std::string& msg )const;

	private:
		virtual bool canLog( const LogFlags& /*flags*/ )const { return false; }
		virtual void log( const std::string& /*msg*/ )const{}
	};

	class LogRouter : 
		public Singleton< LogRouter >
	{
		LogRouter();
		~LogRouter();
		DECL_SINGLETON( LogRouter )

		public:
			void addLogger( boost::shared_ptr< LogModule >& logger );
			void log( const LogFlags& flags, const std::string& msg, ... )const;
			void logNoArg( const LogFlags& flags, const std::string& msg )const;
			void format( std::string& buffer, const std::string& msg, ... )const;

		private:
			std::list< boost::shared_ptr< LogModule > > m_loggers;

	};

	///< flags
	enum{

		logFlag_error = 0,  ///< handles error results
		logFlag_results , ///< handles results no matter if its an error or not
		logFlag_platform , ///< platform spacific things only
		logFlag_system_init , ///< logs out engine specific initializtion
		logFlag_file ///< handles file logging
	};

	template< int bits >
	void log( const std::string& msg, ... )
	{
		boost::int8_t buffer[128];
		va_list args;
		boost::int32_t count = 0;

		va_start( args, msg );
		count = vsprintf( &buffer[0], msg.c_str(), args );
		va_end( args );

		LogRouter::instance().logNoArg( LogFlags(bits), buffer );
	}
}




#define LOG_I Spiral::log< Spiral::Int2Bit<logFlag_system_init>::value >
#define LOG_E Spiral::log< Spiral::Int2Bit<logFlag_error>::value >

#endif