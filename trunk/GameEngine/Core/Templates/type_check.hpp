#ifndef _TYPE_CHECK_H
#define _TYPE_CHECK_H

namespace mpt
{
          namespace type_check
          {
			  template< typename _ty >
			  struct is_pointer
			  {
					 static const bool value = false;
			  };
	          
			  template< typename _ty >
			  struct is_pointer<_ty*>
			  {
					 static const bool value = true;
			  };
	          
	          template< typename _ty >
			  struct is_real
			  {
				  static const bool value = false;
			  };

			  template<>
			  struct is_real< float >
			  {
				  static const bool value = true;
			  };

			  template< typename _ty >
			  struct is_reference
			  {
					 static const bool value = false;
			  };
	          
			  template< typename _ty >
			  struct is_reference< _ty& >
			  {
					 static const bool value = true;
			  };
	          

			  //========================================================
			  // checks for declaration of const
			  template< typename _ty >
			  struct is_const
			  {
					 static const bool value = false;
			  };
	          
			  template< typename _ty >
			  struct is_const< const _ty >
			  {
				  static const bool value = true;
			  };

			  template< typename _ty >
			  struct is_const< const _ty* >
			  {
				  static const bool value = true;
			  };

			  template< typename _ty >
			  struct is_const< const _ty& >
			  {
					 static const bool value = true;
			  };


			  template< typename _ty >
			  struct is_const_reference
			  {
				  static const bool value = 
					  is_const< _ty >::value &&
					  is_reference< _ty >::value;
			  };

          } 
}
#endif
