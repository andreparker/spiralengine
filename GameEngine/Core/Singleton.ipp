#ifndef SINGLETON_IPP
#define SINGLETON_IPP

#include <boost/utility.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

#define DECL_SINGLETON( c )\
	private:\
	friend class Singleton< c >;\
	public:

namespace Spiral
{
	template< class T >
	class Singleton : private boost::noncopyable
	{
		protected:
			Singleton(){}
			~Singleton(){}
			
		public:
			
			static T& instance()
			{
				static T _instance;
				return _instance;
			}
	};
	
	template< class S >
	class SingletonHandle : private boost::noncopyable
	{
	public:
		
		//BOOST_STATIC_ASSERT( boost::is_convertible< S, Singleton< S > >::value == true );
		
		~SingletonHandle(){}
		
		SingletonHandle()
		{
			m_handle = &S::instance();
		}
		
		const S* operator->()const
		{
			return m_handle;
		}
		
		S* operator->()
		{
			return const_cast< S* >( static_cast< const SingletonHandle >(*this).operator->() );
		}
		
		const S& operator*()const
		{
			return *m_handle;
		}
		
		S& operator*()
		{
			return const_cast< S >( *(static_cast< const SingletonHandle >(*this)) );
		}
	private:
		
		S* m_handle;
	};
}

#endif