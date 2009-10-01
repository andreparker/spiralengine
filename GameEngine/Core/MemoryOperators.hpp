#ifndef MEMORY_OPERATORS_HPP
#define MEMORY_OPERATORS_HPP


#if 0
void* __cdecl operator new( size_t bytes );
void* __cdecl operator new[]( size_t bytes );

void __cdecl operator delete( void* ptr );
void __cdecl operator delete[]( void* ptr );


void* __cdecl operator new( size_t bytes, const char* srcFile, int line );
void* __cdecl operator new[]( size_t bytes, const char* srcFile, int line );

void __cdecl operator delete( void* ptr, const char* srcFile, int line );
void __cdecl operator delete[]( void* ptr, const char* srcFile, int line );
#endif

#endif