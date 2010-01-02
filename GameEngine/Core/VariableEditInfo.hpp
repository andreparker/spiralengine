#ifndef VARIABLE_EDIT_INFO_HPP
#define VARIABLE_EDIT_INFO_HPP

#include <string>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>

namespace Spiral
{

	struct VariableEditInfo
	{
		VariableEditInfo( const std::string& name, boost::uint32_t offset, boost::uint32_t size ):
		var_name( name ), var_offset( offset ), var_size( size ){}

		std::string var_name;
		boost::uint32_t var_offset;
		boost::uint32_t var_size;
	};

	typedef boost::function1< void, std::vector< VariableEditInfo >& > VariableEditCallBack;

	// Gets the offset address of a variable in a class/struct
#define OBJ_OFFSET( objName_, varName_ )\
	(reinterpret_cast<boost::uint32_t>( &((objName_*)0x1)->varName_ ) - 1)
#define VAR_SIZE( objName_, varName_ )\
	sizeof( ((objName_*)0x1)->varName_ )

#define DECL_VARIABLE_EDIT( class_ )\
public: static void fill_in_variable_info( std::vector< Spiral::VariableEditInfo >& info );

#define IMPL_VARIABLE_EDIT( class_ )\
	void class_::fill_in_variable_info( std::vector< Spiral::VariableEditInfo >& info )

#define DEF_VARIABLE( class_, varNameStr_ , varName_ )\
	info.push_back( VariableEditInfo( varNameStr_, OBJ_OFFSET( class_, varName_ ), VAR_SIZE( class_, varName_ ) ));
}

#endif