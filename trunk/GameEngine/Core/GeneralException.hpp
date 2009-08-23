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

#endif