#ifndef _TYPE_UTILITY_H
#define _TYPE_UTILITY_H

#include "type_traits.hpp"

namespace mpt
{
	namespace type_utility
	{

		template< typename _ty >
		struct remove_const : 
			public types::type_traits< _ty >{};

		template< typename _ty >
		struct remove_const< const _ty > : 
			public types::type_traits< _ty > {};


		template< typename _ty >
		struct remove_ref : 
			public types::type_traits< _ty > {};

		template< typename _ty >
		struct remove_ref< _ty& > : 
			public types::type_traits< _ty > {};

		//======================================================================
		// Removes a pointer from the data type declaration
		template< typename _ty >
		struct remove_ptr : 
			public types::type_traits< _ty > {};

		template< typename _ty >
		struct remove_ptr< _ty* > : 
			public types::type_traits< _ty > {};




		template< typename _ty >
		struct remove_const_ref : 
			public types::type_traits< 
			typename type_utility::remove_ref< 
			typename type_utility::remove_const<_ty>::type 
			>::type
			> {};

		template< typename _ty >
		struct remove_const_ptr :
			public types::type_traits<
			typename type_utility::remove_ptr<
			typename type_utility::remove_const<_ty>::type
			>::type
			>{};

	}
}

#endif
