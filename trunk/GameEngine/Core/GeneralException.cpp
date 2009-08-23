#include "GeneralException.hpp"

using namespace Spiral;

GeneralException::GeneralException( const std::string& errorStr )throw():
BaseException( "General", errorStr )
{
}

const char* GeneralException::what()const throw()
{
	return GetGeneralError().c_str();
}