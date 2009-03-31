#ifndef TYPES_HPP
#define TYPES_HPP

namespace gameEngine {

	typedef char int8;
	typedef unsigned char uint8;

	typedef short int16;
	typedef unsigned short uint16;

	typedef long int32;
	typedef unsigned long uint32;

	typedef float real;

	struct Size2
	{
		int32 x,y;
	};

	template< int i >
	struct bit
	{
		static const int32 value = 1 << i ; ///< bit values
	};
}

#endif