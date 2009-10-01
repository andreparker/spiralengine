#include <exception>

#include "MemoryOperators.hpp"
#include "MemoryManager.hpp"
#include "MemoryPolicyMalloc.hpp"

using namespace Spiral;

#if 0

void* allocateMem( size_t bytes, const char* srcFile, int line )
{
	void* ptr = MemoryManager< MallocPolicy, 0 >::instance().Alloc( bytes, srcFile, line );
	if( ptr == NULL )
	{
		throw std::bad_alloc();
	}

	return ptr;
}


void* __cdecl operator new( size_t bytes )
{
	return allocateMem( bytes, "UnKnown", 0 );
}

void* __cdecl operator new[]( size_t bytes )
{
	return allocateMem( bytes, "UnKnown", 0 );
}

void __cdecl operator delete( void* ptr )
{
	MemoryManager< MallocPolicy, 0 >::instance().Dealloc( ptr );
}

void __cdecl operator delete[]( void* ptr )
{
	MemoryManager< MallocPolicy, 0 >::instance().Dealloc( ptr );
}


void* __cdecl operator new( size_t bytes, const char* srcFile, int line )
{
	return allocateMem( bytes, srcFile, line );
}

void* __cdecl operator new[]( size_t bytes, const char* srcFile, int line )
{
	return allocateMem( bytes, srcFile, line );
}

void __cdecl operator delete( void* ptr, const char* /*srcFile*/, int /*line*/ )
{
	MemoryManager< MallocPolicy, 0 >::instance().Dealloc( ptr );
}

void __cdecl operator delete[]( void* ptr, const char* /*srcFile*/, int /*line*/ )
{
	MemoryManager< MallocPolicy, 0 >::instance().Dealloc( ptr );
}


#endif