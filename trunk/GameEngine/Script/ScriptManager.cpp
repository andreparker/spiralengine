#include <algorithm>
#include <boost/functional/hash.hpp>
#include <luabind/luabind.hpp>
#include <boost/cstdint.hpp>
#include <lualib.h>
#include <memory.h>
#include <sstream>

#include "ScriptManager.hpp"
#include "../Core/File.hpp"
#include "../Core/Log.hpp"


using namespace Spiral;

namespace
{
	const boost::uint32_t kBufferSize = 512;
	char dataBuffer[ kBufferSize ];

	const char* __cdecl Reader( lua_State* state, void* data, size_t* size )
	{
		IFile* file = reinterpret_cast< IFile* >( data );
		*size = file->Read( reinterpret_cast<boost::int8_t*>( dataBuffer ), kBufferSize );

		return dataBuffer;
	}

	void* __cdecl Alloc_lua ( void *ud, void *ptr, size_t osize, size_t nsize )
	{
		if( nsize )
		{
			void* np = new char[ nsize + sizeof(size_t) ];
			char* addr = (char*)np + sizeof(size_t);

			memset( addr, 0, nsize );
			
			if( ptr )
			{
				char* actual = (char*)((char*)ptr - sizeof(size_t));
				size_t size = *(size_t*)actual;

				memcpy( addr, ptr, ( size > nsize ? nsize : size ) );
				delete [] actual;
			}	
			

			*(size_t*)np = nsize;

			return addr;
		}else
		{
			if( ptr )
			{
				char* actual = (char*)((char*)ptr - sizeof(size_t));
				delete [] actual;
			}
		}

		return NULL;
	}

	int AddDebugInfo(lua_State* L)
	{
		lua_Debug d;
		lua_getstack(L, 1, &d);
		lua_getinfo(L, "Sln", &d);
		std::string err = lua_tostring(L, -1);
		lua_pop(L, 1);
		std::stringstream msg;
		msg << d.short_src << ":" << d.currentline;

		if (d.name != 0)
		{
			msg << "(" << d.namewhat << " " << d.name << ")";
		}

		msg << " " << err;

		LOG_E( "^rScript Error:" + msg.str() + "\n" );
		lua_pushstring(L, msg.str().c_str());
		return 1;
	}

	void close_state( lua_State* s )
	{
		lua_close( s );
	}

}

ScriptManager::ScriptManager():
m_luaState(),
m_hashlist()
{
}


bool ScriptManager::Initialize()
{
	m_luaState.reset( lua_newstate( Alloc_lua, NULL ), close_state );
	
	LOG_I( "^yScriptManager: ^w Initializing Lua modules....\n" );
	// open some standard lua libs
	//luaopen_base( m_luaState.get() );
	//luaopen_string( m_luaState.get() );
	
	// attach lua bind to the state
	luabind::open( m_luaState.get() );
	luabind::set_pcall_callback( AddDebugInfo );

	return true;
}

void ScriptManager::UnInitialize()
{
	m_luaState.reset();
	m_hashlist.clear();
}

bool ScriptManager::LoadScript( const boost::shared_ptr< IFile >& scriptFile, const char* scriptTagName  )
{
	bool isScriptLoaded = true;
	boost::hash< const char* > hash;

	// create a hash value and store it as a unique id for the script
	size_t hashValue = hash( scriptTagName );

	// check to see if its been loaded already
	std::list< size_t >::iterator itr = std::find( m_hashlist.begin(), m_hashlist.end(), hashValue );

	if( itr == m_hashlist.end() )
	{
		if( lua_load( m_luaState.get(), (lua_Reader)Reader, reinterpret_cast<void*>(scriptFile.get()), scriptTagName ) == 0 )
		{
			lua_pcall( m_luaState.get(), 0, LUA_MULTRET, 0 );
			m_hashlist.push_back( hashValue );
		}else
		{
			isScriptLoaded = false;
			LOG_E( "^yScriptManager::LoadScript: ^rFailed loading script %1% !\n", scriptTagName );
		}
	}
	
	return isScriptLoaded;
}

void ScriptManager::RegisterModule( const luabind::scope& s )
{
	luabind::module( m_luaState.get(), "Spiral" )[ s ];
}

