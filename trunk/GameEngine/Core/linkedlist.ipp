#ifndef LINKED_LIST_IPP
#define LINKED_LIST_IPP

#include "types.hpp"
#include "Iterator.ipp"

namespace gameEngine {
	namespace common {

		template< class T > 
		class allocatorStdNew
		{
			public:
				T* operator()();
				void operator()( const T* pointer );
		};

		template< class T > T* allocatorStdNew< T >::operator()()
		{
			return new T;
		}

		template< class T > void allocatorStdNew< T >::operator()( const T* pointer )
		{
			delete pointer;
		}

		template< class T >
		struct element
		{
			T _element;
			typedef T type;
			element* _next;
		};

		template< class T, class Allocator = allocatorStdNew< element< T > > >
		class linkedList
		{
			
			public:
				linkedList();
				~linkedList();

				typedef Iterator< linkedList< T > > const_iterator;
				typedef Iterator< linkedList< T > > iterator;

				void insertHead( const T& item );
				void insertTail( const T& item );

				bool remove( iterator& itr );

				const_iterator end()const;
				iterator end();
				const_iterator head()const;
				iterator head();
				const_iterator tail()const;
				iterator tail();

			protected:
				element< T > *_end;
				element< T > *_newElement( const T& item, element< T >* next );
				element< T > *_head,*_tail;
		};

		
		template< class T, class Allocator >
		bool linkedList< T,Allocator >::remove( Iterator< linkedList< T > >& itr )
		{
			element< T >* e = itr._data;
			element< T >* s = _head;
			element< T >* last = _head;
			Allocator allocator;

			if( _head == e )
			{
				_head = _head->_next;
				allocator( e ); // free the memory
				return true;
			}

			while( s != _end )
			{
				if( s == e )
				{
					last->_next = s->_next;
					allocator( e );
					return true;
				}

				last = s;
				s = s->_next;
			}

			return false;
		}

		template< class T, class Allocator >
		Iterator< linkedList< T > > linkedList< T,Allocator >::end()const
		{
			return const_iterator( _end );
		}

		template< class T, class Allocator >
		Iterator< linkedList< T > > linkedList< T,Allocator >::end()
		{
			return iterator( _end );
		}

		template< class T, class Allocator > 
		Iterator< linkedList< T > > linkedList< T,Allocator >::head()const
		{
			return const_iterator( _head  );
		}

		template< class T, class Allocator >
		Iterator< linkedList< T > > linkedList< T,Allocator >::head()
		{
			return iterator( _head );
		}

		template< class T, class Allocator >
		Iterator< linkedList< T > > linkedList<T,Allocator>::tail()const
		{
			return const_iterator( _tail );
		}

		template< class T, class Allocator >
		Iterator< linkedList< T > > linkedList<T,Allocator>::tail()
		{
			return iterator( _tail );
		}

		template< class T, class Allocator >
		linkedList<T,Allocator>::linkedList()
		{
			Allocator allocator;
			_end = allocator();
			_head = _tail = _end;
		}

		template< class T, class Allocator >
		linkedList<T,Allocator>::~linkedList()
		{
			Allocator allocator;
			if( _head )
			{
				element< T >* itr = _head;
				while( itr != _end )
				{
					itr = itr->_next;
					allocator( _head );
					_head = itr;
				}
				_head = _tail = 0;
			}

			if( _end )
				allocator( _end );
		}

		template< class T, class Allocator >
		element< T >* 
			linkedList<T,Allocator>::_newElement( const T& item, element< T >* next )
		{
			Allocator allocator;
			element< T >* newElement = allocator();
			newElement->_element = item;
			newElement->_next = next;

			return newElement;
		}

		template< class T, class Allocator >
		void linkedList<T,Allocator>::insertHead( const T& item )
		{
			if( _head != _end )
			{
				_head = _newElement( item, _head );
			}
			else
			{
				_head = _newElement( item, _end );
				_tail = _head;
			}
		}

		template< class T, class Allocator >
		void linkedList<T,Allocator>::insertTail( const T& item )
		{
			if( _head != _end )
			{
				_tail->_next = _newElement( item, _end );
				_tail = _tail->_next;
			}else
			{
				_head = _newElement( item, _end );
				_tail = _head;
			}
		}


		template< class T >
		struct Iterator< linkedList< T > >
		{
			public:
				friend class linkedList< T >;
				Iterator():_data(0){}
				explicit Iterator( const element< T >*& type );
				explicit Iterator( element< T >* const& type );
				Iterator( const Iterator< linkedList< T > >& itr );
				~Iterator();

				typedef element< T > _element;
				typedef Iterator< linkedList< T > > const_iterator;
				typedef Iterator< linkedList< T > > iterator;

				operator const _element*()const { return _data; }
				operator _element*() { return _data; }
				const T& operator*()const;
				T& operator*();

				void operator=( const Iterator< linkedList< T > >& itr );

				iterator operator++();
				iterator operator++(int);
				const_iterator operator++()const;
				const_iterator operator++(int)const;

				//============================================

				const_iterator operator+(int)const;
				iterator operator+(int);

				//=============================================
				bool operator==(const Iterator< linkedList< T > >& itr)const;
				bool operator!=(const Iterator< linkedList< T > >& itr)const;
			protected:

				const _element* _moveNext( _element* e, int32 count );
				mutable _element* _data;
		};

		template< class T >
		void Iterator< linkedList< T > >::operator=( const Iterator< linkedList< T > >& itr )
		{
			_data = itr._data;
		}

		template< class T >
		Iterator< linkedList< T > >::Iterator( const Iterator< linkedList< T > >& itr )
		{
			_data = itr._data;
		}

		template< class T >
		Iterator< linkedList< T > >::Iterator( const element< T >*& type )
		{
			_data = type;
		}
		
		template< class T >
		Iterator< linkedList< T > >::Iterator( element< T >* const& type )
		{
			_data = type;
		}



		template< class T >
		Iterator< linkedList< T > >::~Iterator()
		{
		}

		template< class T >
		inline const T& Iterator< linkedList< T > >::operator*()const
		{
			return _data->_element;
		}

		template< class T >
		inline T& Iterator< linkedList< T > >::operator*()
		{
			return _data->_element;
		}

		template< class T >
		inline Iterator< linkedList< T > > 
			Iterator< linkedList< T > >::operator++()
		{
			_data = _data->_next;
			return  iterator( _data );
		}

		template< class T >
		inline Iterator< linkedList< T > > 
			Iterator< linkedList< T > >::operator++()const
		{
			_data = _data->_next;
			return  const_iterator( _data );
		}

		template< class T >
		inline Iterator< linkedList< T > > 
			Iterator< linkedList< T > >::operator++( int i )
		{
			iterator tmp( _data );
			_data = _data->_next;

			return tmp;
		}

		template< class T >
		inline Iterator< linkedList< T > > 
			Iterator< linkedList< T > >::operator++( int i )const
		{
			const_iterator tmp( _data );
			_data = _data->_next;

			return tmp;
		}
		
		//============================================

		template< class T >
		inline const element< T >* 
			Iterator< linkedList< T > >::_moveNext( element< T >* e, int32 count )
		{
			if( count > 0 )
			{
				for( int32 i = 0; i < count; ++i )
				e = e->_next;
			}
			
			return e;
		}

		template< class T >
		inline Iterator< linkedList< T > > 
			Iterator< linkedList< T > >::operator+( int i )const
		{
			const_iterator itr( _moveNext( _data, i ) );
			return itr;
		}

		template< class T >
		inline Iterator< linkedList< T > > 
			Iterator< linkedList< T > >::operator+( int i )
		{
			iterator itr( _moveNext( _data, i ) );
			return itr;
		}

		//=============================================
		template< class T >
		inline bool Iterator< linkedList< T > >::operator==( const Iterator< linkedList< T > >& itr)const
		{
			return bool( _data == itr._data );
		}

		template< class T >
		inline bool Iterator< linkedList< T > >::operator!=( const Iterator< linkedList< T > >& itr)const
		{
			return bool( _data != itr._data ); 
		}
	}
}
#endif