#ifndef SCRIPT_UTILS_HPP
#define SCRIPT_UTILS_HPP

#include <boost/shared_ptr.hpp>

namespace Spiral
{
	class ScriptManager;

	void RegisterScriptUtils( boost::shared_ptr<ScriptManager>& scriptMgr );
}
#endif