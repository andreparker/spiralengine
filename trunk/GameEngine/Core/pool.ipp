#ifndef POOL_HPP
#define POOL_HPP

#include "types.hpp"


namespace gameEngine
{
	namespace common
	{
		// simple pool class
		template< class T >
		class pool
		{
			struct link
			{
				int32 used : 1;
				link* next : 31;
				T object;
			};
			public:
				pool();
				~pool();

				T* alloc();
				void free( T* ptr );
				int32 totalAllocated();

			protected:
				link* _newLink();
				link* _findFree();
				void _addLink( link* lnk );

				link* _head;
				link* _tail;
				int32 _totalAllocated;
		};

		template< class T >
		pool<T>::pool():_head( 0 ),_tail( 0 ),_totalAllocated( 0 ){}

		template< class T >
		pool<T>::~pool()
		{
			link* lnk = _head;
			while( lnk )
			{
				_head = lnk->next;
				delete lnk;
				lnk = _head;
			}
			_head = _tail = 0;
		}

		template< class T >
		int32 pool<T>::totalAllocated()
		{
			return _totalAllocated;
		}

		template< class T >
		void pool<T>::_addLink( pool<T>::link* lnk )
		{
			if( _head )
			{
				_tail->next = lnk;
				_tail = _tail->next;
			}else
			{
				_head = lnk;
				_tail = lnk;
			}
		}

		template< class T >
		T* pool<T>::alloc()
		{
			link* freePtr = _findFree();
			if( !freePtr )
			{
				freePtr = _newLink();
				_addLink( freePtr );
			}

			return &freePtr->object;
		}

		template< class T >
		void pool<T>::free( T* ptr )
		{
			link* lnk = _head;
			while( lnk )
			{
				if( (&lnk->object) == ptr )
				{
					lnk->used = 0;
					break;
				}

				lnk = lnk->next;
			}
		}

		template< class T >
		pool<T>::link* pool<T>::_newLink()
		{
			link* newLink = new link;
			newLink->next = 0;
			newLink->used = 1;
			++_totalAllocated;

			return newLink;
		}

		template< class T >
		pool<T>::link* pool<T>::_findFree()
		{
			link* lnk = _head;
			while( lnk )
			{
				if( !lnk->used )
				{
					lnk->used = 1;
					return lnk;
				}

				lnk = lnk->next;
			}

			return 0;
		}

	}
}
#endif