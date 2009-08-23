#include "WinOglException.hpp"

using namespace Spiral;
WinOglException::WinOglException( const std::string& errorStr ):
Spiral::BaseException( "Gfx:WinOglDriver", errorStr )
{
}

const char* WinOglException::what()const throw()
{
	std::string& final = GetGeneralError();
    return final.c_str();
}