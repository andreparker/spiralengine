#ifndef ITERATOR_IPP
#define ITERATOR_IPP

namespace gameEngine {
	namespace common {

		template< class T >
		struct Iterator
		{
			public:
				Iterator( T& type );
				Iterator( const Iterator<T>& itr );
				~Iterator();

				typedef const Iterator< T > const_iterator;
				typedef Iterator< T > iterator;

				const T& operator*()const;
				T& operator*();

				const_iterator& operator++()const;
				iterator& operator++();

				const_iterator operator++(int)const;
				iterator operator++(int);

				//============================================

				const_iterator& operator--()const;
				iterator& operator--();

				const_iterator operator--(int)const;
				iterator operator--(int);

				//============================================

				const_iterator operator+(int)const;
				iterator operator+(int);

				const_iterator operator-(int)const;
				iterator operator-(int);

				//=============================================
				bool operator==(const_iterator&)const;
				bool operator!=(const_iterator&)const;
			protected:
				T* _data;
		};

		template< class T >
		Iterator< T >::Iterator( T& type )
		{
			_data = type;
		}

		template< class T >
		Iterator< T >::Iterator( const Iterator<T>& itr )
		{
			_data = itr._data;
		}

		template< class T >
		Iterator< T >::~Iterator()
		{
		}

		template< class T >
		const T& Iterator< T >::operator*()const
		{
			return *_data;
		}

		template< class T >
		T& Iterator< T >::operator*()
		{
			return *_data;
		}

		template< class T >
		const Iterator< T >& Iterator< T >::operator++()const
		{
			++_data;
			return *this;
		}

		template< class T >
		Iterator< T >& Iterator< T >::operator++()
		{
			++_data;
			return *this;
		}

		template< class T >
		const Iterator< T > Iterator< T >::operator++( int i )const
		{
			const_iterator tmp( _data );
			++_data;
			return tmp;
		}

		template< class T >
		Iterator< T > Iterator< T >::operator++( int i )
		{
			iterator tmp( _data );
			++_data;
			return  tmp;
		}

		//============================================

		template< class T >
		const Iterator< T >& Iterator< T >::operator--()const
		{
			--_data;
			return *this;
		}

		template< class T >
		Iterator< T >& Iterator< T >::operator--()
		{
			--_data;
			return *this;
		}

		template< class T >
		const Iterator< T > Iterator< T >::operator--( int i )const
		{
			const_iterator itr( _data );
			--_data;
			return itr;
		}

		template< class T >
		Iterator< T > Iterator< T >::operator--( int i )
		{
			iterator itr( _data );
			--_data;
			return itr;
		}

		//============================================

		template< class T >
		const Iterator< T > Iterator< T >::operator+( int i )const
		{
			return const_iterator( _data + i );
		}

		template< class T >
		Iterator< T > Iterator< T >::operator+( int i )
		{
			return iterator( _data + i );
		}

		template< class T >
		const Iterator< T > Iterator< T >::operator-( int i )const
		{
			return const_iterator( _data - i );
		}

		template< class T >
		Iterator< T > Iterator< T >::operator-( int i )
		{
			return iterator( _data - i );
		}

		//=============================================
		template< class T >
		bool Iterator< T >::operator==( const_iterator& itr)const
		{
			return bool( *_data == *itr._data );
		}

		template< class T >
		bool Iterator< T >::operator!=(const_iterator& itr)const
		{
			return bool( *_data == *itr._data );
		}
	}
}
#endif