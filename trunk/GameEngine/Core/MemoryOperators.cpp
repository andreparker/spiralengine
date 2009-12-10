#include <exception>

#include "MemoryOperators.hpp"
#include "MemoryManager.hpp"
#include "MemoryPolicyMalloc.hpp"

using namespace Spiral;

#undef new

#if 0

void* allocateMem( size_t bytes )
{
	void* ptr = MemoryManager< MallocPolicy, 0 >::instance().Alloc( bytes );
	if( ptr == NULL )
	{
		throw std::bad_alloc();
	}

	return ptr;
}


void* __cdecl operator new( size_t bytes )
{
	return allocateMem( bytes );
}

void* __cdecl operator new[]( size_t bytes )
{
	return allocateMem( bytes );
}

void __cdecl operator delete( void* ptr )
{
	MemoryManager< MallocPolicy, 0 >::instance().Dealloc( ptr );
}

void __cdecl operator delete[]( void* ptr )
{
	MemoryManager< MallocPolicy, 0 >::instance().Dealloc( ptr );
}



#endif