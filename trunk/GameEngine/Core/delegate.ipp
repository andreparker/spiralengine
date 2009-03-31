#ifndef DELEGATE_IPP
#define DELEGATE_IPP

namespace gameEngine
{
	namespace common
	{

		template< class R >
		class delegate0
		{
			protected:
				typedef R (*CallMethod)( void* data );
				void* m_caller;
				CallMethod m_funcMethod;

			public:
				delegate0():m_caller(NULL){}

				template< class Object, R (Object::*Func)() >
				static delegate0<R> bind( Object* obj )
				{
					delegate0<R> d;
					d.m_caller = reinterpret_cast< void* >( obj );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &class_func_method< Object, Func > );
					
					return d;
				}

				static delegate0<R> function( R (*func)() )
				{
					delegate0<R> d;
					d.m_caller = reinterpret_cast< void* >( func );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &func_method );

					return d;
				}

				R operator()()
				{
					return (*m_funcMethod)( m_caller );
				}

			protected:

				static R func_method( void* data )
				{
					typedef R (*Function)();

					Function caller = reinterpret_cast< Function >( data );
					return (*caller)();
				} 

				template< class Object, R (Object::*Function)() >
				static R class_func_method( void* data )
				{
					Object* obj = static_cast< Object* >( data );
					return (obj->*Function)();
				}

		};


		//================================================================================================
		template< class Param0 >
		class delegate1
		{
			protected:
				typedef void (*CallMethod)( void* data, Param0  );
				void* m_caller;
				CallMethod m_funcMethod;

			public:
				delegate1():m_caller(NULL){}

				template< class Object, void (Object::*Func)( Param0 ) >
				static delegate1< Param0 > bind( Object* obj )
				{
					delegate1< Param0 > d;
					d.m_caller = reinterpret_cast< void* >( obj );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &class_func_method< Object, Func > );
					
					return d;
				}

				static delegate1< Param0 > function( void (*func)( Param0 ) )
				{
					delegate1< Param0 > d;
					d.m_caller = reinterpret_cast< void* >( func );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &func_method );

					return d;
				}

				void operator()( Param0 p0 )
				{
					return (*m_funcMethod)( m_caller, p0 );
				}

			protected:
				static void func_method( void* data, Param0 p0 )
				{
					typedef void (*Function)( Param0 );

					Function caller = reinterpret_cast< Function >( data );
					return (*caller)( p0 );
				}

				template< class Object, void (Object::*Function)( Param0 ) >
				static void class_func_method( void* data, Param0 p0 )
				{
					Object* obj = static_cast< Object* >( data );
					return (obj->*Function)( p0 );
				}

		};

		//=======================================================================================================

		template< class Param0, class Param1 >
		class delegate2
		{
			protected:
				typedef void (*CallMethod)( void* data, Param0, Param1  );
				void* m_caller;
				CallMethod m_funcMethod;

			public:
				delegate2():m_caller(NULL){}

				template< class Object, void (Object::*Func)( Param0, Param1 ) >
				static delegate2<Param0,Param1> bind( Object* obj )
				{
					delegate2<Param0,Param1> d;
					d.m_caller = reinterpret_cast< void* >( obj );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &class_func_method< Object, Func > );
					
					return d;
				}

				static delegate2< Param0,Param1 > function( void (*func)( Param0, Param1 ) )
				{
					delegate2< Param0,Param1 > d;
					d.m_caller = reinterpret_cast< void* >( func );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &func_method );

					return d;
				}

				void operator()( Param0 p0, Param1 p1 )
				{
					return (*m_funcMethod)( m_caller, p0, p1 );
				}

			protected:
				static void func_method( void* data, Param0 p0,Param1 p1 )
				{
					typedef void (*Function)( Param0,Param1 );

					Function caller = reinterpret_cast< Function >( data );
					return (*caller)( p0, p1 );
				}

				template< class Object, void (Object::*Function)( Param0, Param1 ) >
				static void class_func_method( void* data, Param0 p0, Param1 p1 )
				{
					Object* obj = static_cast< Object* >( data );
					return (obj->*Function)( p0, p1 );
				}

		};

		//========================================================================================================
		template< class Param0, class Param1, class Param2 >
		class delegate3
		{
			protected:
				typedef void (*CallMethod)( void* data, Param0, Param1, Param2  );
				void* m_caller;
				CallMethod m_funcMethod;

			public:
				delegate3():m_caller(NULL){}

				template< class Object, void (Object::*Func)( Param0, Param1, Param2 ) >
				static delegate3<Param0,Param1,Param2> bind( Object* obj )
				{
					delegate3<Param0,Param1,Param2> d;
					d.m_caller = reinterpret_cast< void* >( obj );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &class_func_method< Object, Func > );
					
					return d;
				}

				static delegate3< Param0,Param1,Param2 > function( void (*func)( Param0, Param1, Param2 ) )
				{
					delegate3< Param0,Param1,Param2 > d;
					d.m_caller = reinterpret_cast< void* >( func );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &func_method );

					return d;
				}

				void operator()( Param0 p0, Param1 p1, Param2 p2 )
				{
					return (*m_funcMethod)( m_caller, p0, p1, p2 );
				}

			protected:
				static void func_method( void* data, Param0 p0,Param1 p1, Param2 p2 )
				{
					typedef void (*Function)( Param0,Param1,Param2 );

					Function caller = reinterpret_cast< Function >( data );
					return (*caller)( p0, p1, p2 );
				}

				template< class Object, void (Object::*Function)( Param0, Param1, Param2 ) >
				static void class_func_method( void* data, Param0 p0, Param1 p1, Param2 p2 )
				{
					Object* obj = static_cast< Object* >( data );
					return (obj->*Function)( p0, p1, p2 );
				}

		};

		//========================================================================================================
		template< class Param0, class Param1, class Param2, class Param3 >
		class delegate4
		{
			protected:
				typedef void (*CallMethod)( void* data, Param0, Param1, Param2, Param3  );
				void* m_caller;
				CallMethod m_funcMethod;

			public:
				delegate4():m_caller(NULL){}

				template< class Object, void (Object::*Func)( Param0, Param1, Param2, Param3 ) >
				static delegate4<Param0,Param1,Param2,Param3> bind( Object* obj )
				{
					delegate4<Param0,Param1,Param2,Param3> d;
					d.m_caller = reinterpret_cast< void* >( obj );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &class_func_method< Object, Func > );
					
					return d;
				}

				static delegate4< Param0,Param1,Param2,Param3 > function( void (*func)( Param0, Param1, Param2, Param3 ) )
				{
					delegate4< Param0,Param1,Param2,Param3 > d;
					d.m_caller = reinterpret_cast< void* >( func );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &func_method );

					return d;
				}

				void operator()( Param0 p0, Param1 p1, Param2 p2, Param3 p3 )
				{
					return (*m_funcMethod)( m_caller, p0, p1, p2, p3 );
				}

			protected:
				
				static void func_method( void* data, Param0 p0,Param1 p1, Param2 p2, Param3 p3 )
				{
					typedef void (*Function)( Param0,Param1,Param2,Param3 );

					Function caller = reinterpret_cast< Function >( data );
					return (*caller)( p0, p1, p2, p3 );
				}

				template< class Object, void (Object::*Function)( Param0, Param1, Param2, Param3 ) >
				static void class_func_method( void* data, Param0 p0, Param1 p1, Param2 p2, Param3 p3 )
				{
					Object* obj = static_cast< Object* >( data );
					return (obj->*Function)( p0, p1, p2, p3 );
				}

		};

		//========================================================================================================
		template< class Param0, class Param1, class Param2, class Param3, class Param4 >
		class delegate5
		{
			protected:
				typedef void (*CallMethod)( void* data, Param0, Param1, Param2, Param3, Param4  );
				void* m_caller;
				CallMethod m_funcMethod;

			public:
				delegate5():m_caller(NULL){}

				template< class Object, void (Object::*Func)( Param0, Param1, Param2, Param3, Param4 ) >
				static delegate5<Param0,Param1,Param2,Param3,Param4> bind( Object* obj )
				{
					delegate5<Param0,Param1,Param2,Param3,Param4> d;
					d.m_caller = reinterpret_cast< void* >( obj );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &class_func_method< Object, Func > );
					
					return d;
				}

				static delegate5< Param0,Param1,Param2,Param3,Param4 > function( void (*func)( Param0, Param1, Param2, Param3, Param4 ) )
				{
					delegate5< Param0,Param1,Param2,Param3,Param4 > d;
					d.m_caller = reinterpret_cast< void* >( func );
					d.m_funcMethod = reinterpret_cast< CallMethod >( &func_method );

					return d;
				}

				void operator()( Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4 )
				{
					return (*m_funcMethod)( m_caller, p0, p1, p2, p3, p4 );
				}

			protected:
				static void func_method( void* data, Param0 p0,Param1 p1, Param2 p2, Param3 p3, Param4 p4 )
				{
					typedef void (*Function)( Param0,Param1,Param2,Param3,Param4 );

					Function caller = reinterpret_cast< Function >( data );
					return (*caller)( p0, p1, p2, p3, p4 );
				}

				template< class Object, void (Object::*Function)( Param0, Param1, Param2, Param3, Param4 ) >
				static void class_func_method( void* data, Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4 )
				{
					Object* obj = static_cast< Object* >( data );
					return (obj->*Function)( p0, p1, p2, p3, p4 );
				}

		};
	}
}

#endif