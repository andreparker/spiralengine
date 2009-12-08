#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "singleton.ipp"
#include "TypeUtils.hpp"

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
		
		DECL_SINGLETON( LogRouter )

		public:
			~LogRouter();
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
	void log( const std::string& msg )
	{	
		LogRouter::instance().logNoArg( LogFlags(bits), msg );
	}

	template< int bits, class Arg1 >
	void log( const std::string& msg, Arg1 arg1 )
	{
		boost::format frmt( msg.c_str() );
		frmt % arg1;

		log<bits>( frmt.str() );
	}

	template< int bits, class Arg1, class Arg2 >
	void log( const std::string& msg, Arg1 arg1, Arg2 arg2 )
	{
		boost::format frmt( msg.c_str() );
		frmt % arg1 % arg2;

		log<bits>( frmt.str() );
	}

	template< int bits, class Arg1, class Arg2, class Arg3 >
	void log( const std::string& msg, Arg1 arg1, Arg2 arg2, Arg3 arg3 )
	{
		boost::format frmt( msg.c_str() );
		frmt % arg1 % arg2 % arg3;

		log<bits>( frmt.str() );
	}

	template< int bits, class Arg1, class Arg2, class Arg3, class Arg4 >
	void log( const std::string& msg, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4 )
	{
		boost::format frmt( msg.c_str() );
		frmt % arg1 % arg2 % arg3 % arg4;

		log<bits>( frmt.str() );
	}
	
}




#define LOG_I Spiral::log< Spiral::Int2Bit<logFlag_system_init>::value >
#define LOG_E Spiral::log< Spiral::Int2Bit<logFlag_error>::value >
#define LOG_P Spiral::log< Spiral::Int2Bit<logFlag_platform>::value >

#ifdef _DEBUG
#define LOG_D Spiral::log< Spiral::Int2Bit<logFlag_results>::value >
#else 
#define LOG_D (void)
#endif

#endif