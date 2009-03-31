#ifndef _TYPE_TRAITS_H
#define _TYPE_TRAITS_H

namespace mpt
{
	namespace types
	{

		template< class T >
		class Type2Class
		{
				Type2Class( const T& ){}
				Type2Class<T>& operator=( const T& ){ return *this; }
			public:
				typedef T type;
				type value;
		};

		template< class T >
		struct Type2Type
		{
			typedef T type;
		};

		template< int I >
		struct Int2Type
		{
			enum{
				value = I
			};
		};

		template< typename _ty >
		struct type_traits
		{
			typedef _ty type;
			typedef const _ty const_type;
			typedef _ty* ptr_type;
			typedef const _ty* const_ptr_type;
			typedef _ty& ref_type;
			typedef const _ty& const_ref_type;
			typedef _ty val_type;
			typedef const _ty const_val_type;
		};

		template< typename _ty >
		struct type_traits< _ty* >
		{
			typedef _ty* type;
			typedef const _ty* const_type;
			typedef _ty** ptr_type;
			typedef const _ty** const_ptr_type;
			typedef _ty*& ref_type;
			typedef const _ty*& const_ref_type;
			typedef _ty val_type;
			typedef const _ty const_val_type;
		};

		template< typename _ty >
		struct type_traits< _ty& >
		{
			typedef _ty& type;
			typedef const _ty& const_type;
			typedef _ty* ptr_type;
			typedef const _ty* const_ptr_type;
			typedef _ty& ref_type;
			typedef const _ty& const_ref_type;
			typedef _ty val_type;
			typedef const _ty const_val_type;
		};
	}

}

#endif

