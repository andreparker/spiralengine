/*!
*/
#ifndef WINDOW_EXCEPTION_HPP
#define WINDOW_EXCEPTION_HPP

#include "../../Core/Sp_Exception.hpp"

class WindowException : public Spiral::BaseException
{
public:
	WindowException( const std::string& errorStr );

	virtual const char* what()const throw();
};

#endif