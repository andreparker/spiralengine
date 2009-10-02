/*!
*/
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <list>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>

#include "Sp_DataTypes.hpp"

namespace Spiral
{
	class Engine;
	class Application : private boost::noncopyable
	{
	public:
		/*!
		   @function  Init
		   @brief     application initialization
		   @return    bool
		   @param     boost::int32_t argc - command argument coutn
		   @param     std::list< boost::shared_array< char > > & argList - arguments, a list of char pointers
		   @param     boost::shared_ptr< Engine > & engine
		*/
		bool Init( boost::int32_t argc, std::list< boost::shared_array< char > >& argList, boost::shared_ptr< Spiral::Engine >& engine );
		
		/*!
		   @function  Run
		   @brief     application main
		   @return    bool
		   @param     SpReal ticks - current elapsed time each frame
		   @param     boost::shared_ptr< Engine > & engine
		*/
		bool Run( SpReal ticks, boost::shared_ptr< Spiral::Engine >& engine );

		/*!
		   @function  UnInit
		   @brief     clean up function for the application
		   @return    bool
		*/
		bool UnInit();

	private:
		virtual bool DoInit( boost::int32_t argc, std::list< boost::shared_array< char > >& argList, boost::shared_ptr< Spiral::Engine >& engine ) = 0;
		virtual bool DoRun( SpReal ticks, boost::shared_ptr< Spiral::Engine >& engine ) = 0;
		virtual bool DoUnInit() = 0;
	};
}

/*!
	@brief This is used to specify your application to the engine
*/
#define SP_DECL_APPLICATION( App )\
namespace Spiral\
{\
	boost::shared_ptr<Application> CreateApp()\
	{\
        boost::shared_ptr<Application> app(new App);\
        return app;\
	}\
}

#endif