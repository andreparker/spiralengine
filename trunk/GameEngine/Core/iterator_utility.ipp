#ifndef ITERATOR_UTILITY_IPP
#define ITERATOR_UTILITY_IPP

#include "Iterator.ipp"

namespace gameEngine {
	namespace common {

		template< class T, class Functor >
		Iterator<T> find_if( const Iterator<T>& start, const Iterator<T>& end, Functor func )
		{
			const Iterator<T> itr = start;
			for(; itr != end; ++itr )
			{
				if( func( *itr ) )
				{
					break;
				}
			}

			return itr;
		}

		template< class T, class Functor >
		void for_each( const Iterator<T>& start, const Iterator<T>& end, Functor func )
		{
			const Iterator<T> itr = start;
			for(; itr != end; ++itr )
			{
				func( *itr );
			}
		}

	}
}
#endif