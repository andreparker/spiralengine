#ifndef SINGLETON_IPP
#define SINGLETON_IPP

#define DECL_SINGLETON( c )\
	private:\
	friend class singleton< c >;\
	c( const c& ){}\
	c& operator=( const c& ){ return *this; }\
	public:

namespace gameEngine
{
	template< class T >
	class singleton
	{
		protected:
			singleton(){}
			~singleton(){}
			singleton( const singleton& ){}
			singleton& operator=( const singleton& ){ return *this; }

		public:
			
			static T& instance()
			{
				static T _instance;
				return _instance;
			}
	};
}

#endif