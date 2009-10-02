#include "WindowException.hpp"
#include <Windows.h>

using namespace std;
WindowException::WindowException(const string &errorStr) : 
Spiral::BaseException( "Platform:Windows", errorStr )
{
}

const char* WindowException::what() const throw()
{
	char outBuffer[128];
	ZeroMemory( outBuffer, 128 );
	DWORD error = GetLastError();
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, outBuffer, 128, NULL );

	std::string& final = GetGeneralError();
	final += outBuffer;

	return final.c_str();
}