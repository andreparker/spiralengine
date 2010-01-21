#include "Utf8.hpp"
#include "../../Core/GeneralException.hpp"

namespace Spiral { namespace Common { namespace String {

	wchar_t Make( unsigned char a, unsigned char b )
	{
		return (( a - 192 ) << 6 ) + ( b - 128 );
	}

	wchar_t Make( unsigned char a, unsigned char b, unsigned char c )
	{
		return (( a - 224 ) << 12 ) + (( b - 128 ) << 6) + ( c - 128 );
	}

	wchar_t Make( unsigned char a, unsigned char b, unsigned char c, unsigned char d )
	{
		return (( a - 240 ) << 18 ) + (( b - 128 ) << 12) + ( ( c - 128 ) << 6 ) + ( d - 128 );
	}


	wString ConvertUtf8_to_Wchar( const cString& str )
	{
		wString wstr;
		const char* itr = str.c_str();
		unsigned char a,b,c;

		while( *itr != NULL )
		{
			c = static_cast<unsigned char>(*itr++);

			if( c <= 127 )
			{
				wstr.push_back( c );
			}else if( c >= 192 && c <= 223 )
			{
				a = static_cast<unsigned char>(*itr++);
				wstr.push_back( Make( c , a ) );
			}else if( c >= 224 && c <= 239 )
			{
				a = static_cast<unsigned char>(*itr++);
				b = static_cast<unsigned char>(*itr++);
				wstr.push_back( Make( c, a, b ) );
			}else
			{
				THROW_GENERAL_EXCEPTION( "Error, encoding to larger" );
			}
		}

		wstr.push_back( NULL );
		return wstr;
	}

} 

}

}