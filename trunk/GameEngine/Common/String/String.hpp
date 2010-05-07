#ifndef STRING_HPP
#define STRING_HPP

#include "../../Core/Sp_DataTypes.hpp"

namespace Spiral { namespace Common { namespace String {

	template<  class OutStr , class InStr >
	const OutStr StringToString( const InStr& str )
	{
		typedef typename OutStr::value_type val_type;
		typedef typename InStr::const_iterator const_iterator;
		
		OutStr out;

		for( const_iterator itr = str.begin();
			 itr != str.end(); ++itr )
		{
			 const val_type val = static_cast< const val_type >( *itr );
			 out.push_back( val );
		}
		
		out.push_back( NULL );
		return out;
	}

	inline const wString ConvertCString_To_WString( const cString& str )
	{
		return StringToString< wString >( str );
	}

	inline const cString ConvertWString_To_CString( const wString& str )
	{
		return StringToString< cString >( str );
	}
} // String
} // Common

}
#endif