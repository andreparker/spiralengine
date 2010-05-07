#include "ScriptUtils.hpp"
#include "ScriptManager.hpp"
#include "../Core/Sp_DataTypes.hpp"

using namespace Spiral;

namespace
{

	template< class inStr,class outStr, class inCharType >
	outStr to_string( const inStr& str )
	{
		outStr out;
		inCharType* pStr = const_cast< inCharType* >( str.c_str() );

		while( *pStr != NULL )
		{
			out.push_back( static_cast< typename outStr::value_type >( *pStr++ ) );
		}

		out.push_back( NULL );
		return out;
	}

	luabind::scope RegisterStringUtils()
	{
		luabind::scope s = luabind::namespace_("String")
		[
			luabind::def( "to_wstring", &to_string< cString,wString,cString::value_type >  ),
			luabind::def( "to_cstring", &to_string< wString,cString,wString::value_type > )
		];

		return s;
	}

}

void Spiral::RegisterScriptUtils( boost::shared_ptr<ScriptManager>& scriptMgr )
{
	scriptMgr->RegisterModule( RegisterStringUtils() );
}