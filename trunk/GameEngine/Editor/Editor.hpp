#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../Spiral.hpp"

#include <boost/weak_ptr.hpp>

namespace Editor
{
	class App : public Spiral::Application
	{
	public:
		App();

	private:

		virtual bool DoInit( boost::int32_t argc, std::list< boost::shared_array< char > >& argList, boost::shared_ptr< Spiral::Engine >& engine );
		virtual bool DoRun( Spiral::SpReal ticks, boost::shared_ptr< Spiral::Engine >& engine );
		virtual bool DoUnInit();

	private:

		boost::weak_ptr< Spiral::Engine > m_engine;

	};
}

#endif