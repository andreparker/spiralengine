#ifndef ENGINE_ASSERT_HPP
#define ENGINE_ASSERT_HPP

#include <assert.h>

template< bool T > struct CompileTimeAssert;
template<> struct CompileTimeAssert< false >{};

// this is used so the compiler can fully expand defines before final concatination
#define CONCAT( u,v )  CONCAT2( u, v )
#define CONCAT2( w,x ) CONCAT3( w,x )
#define CONCAT3( y,z ) y##z

#ifdef _DEBUG
#define STATIC_ASSERT( c ) CompileTimeAssert< !(c)  > CONCAT( compile_error_line_, __LINE__ );
#else
#define STATIC_ASSERT( c )
#endif

#define ASSERT( c ) assert( c )

#endif