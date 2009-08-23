#include "WindowException.hpp"

using namespace std;
WindowException::WindowException(const string &errorStr) : 
Spiral::BaseException( "Platform:Windows", errorStr )
{
}

const char* WindowException::what() const throw()
{
	std::string& final = GetGeneralError();
	return final.c_str();
}