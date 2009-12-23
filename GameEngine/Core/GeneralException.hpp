/*!
*/
#ifndef GENERAL_EXCEPTION_HPP
#define GENERAL_EXCEPTION_HPP

#include "Sp_Exception.hpp"

namespace Spiral
{
	class GeneralException : public BaseException
	{
	public:
		GeneralException( const std::string& errorStr )throw();

		virtual const char* what()const throw();
	};
}

#define THROW_GENERAL_EXCEPTION( str ) THROW_BASIC_EXCEPTION( Spiral::GeneralException( str ) )

#endif