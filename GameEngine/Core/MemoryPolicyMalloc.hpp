#ifndef MEMORY_POLICY_MALLOC_HPP
#define MEMORY_POLICY_MALLOC_HPP

#include <boost/cstdint.hpp>
#include <stdlib.h>

namespace Spiral
{
	struct MallocPolicy
	{
		void* Alloc( boost::uint32_t bytes )
		{
			return malloc( bytes );
		}

		void Dealloc( void* ptr )
		{
			free( ptr );
		}

		bool Initialize( boost::uint32_t /*bytes*/ )
		{
			return true;
		}

		void UnInitialize()
		{

		}

	};
}
#endif