#ifndef SCRIPT_MANAGER_HPP
#define SCRIPT_MANAGER_HPP

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>
#include <luabind/luabind.hpp>
#include <lua.h>
#include <list>

namespace Spiral
{
	class IFile;

	class ScriptManager : boost::noncopyable
	{
	public:
		ScriptManager();

		bool Initialize();
		void UnInitialize();

		bool LoadScript( const boost::shared_ptr< IFile >& scriptFile, const char* scriptTagName );

		void RegisterModule( const luabind::scope& s );

		const lua_State* GetLuaState()const
		{
			BOOST_ASSERT( m_luaState );
			return m_luaState.get();
		}

		lua_State* GetLuaState()
		{
			return const_cast< lua_State* >( static_cast< const ScriptManager* >( this )->GetLuaState() );
		}
	private:
		boost::shared_ptr< lua_State > m_luaState;
		std::list< size_t > m_hashlist;
	};
}
#endif