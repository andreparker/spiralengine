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


#ifdef _MSC_VER
#include <eh.h>

std::string TranslateExceptionCode( DWORD );
void straight_to_debugger(unsigned int i, _EXCEPTION_POINTERS* exp )
{ 
	THROW_WINDOW_EXCEPTION( TranslateExceptionCode( exp->ExceptionRecord->ExceptionCode ) ); 
}

std::string TranslateExceptionCode( DWORD code )
{
	switch( code )
	{
	case EXCEPTION_ACCESS_VIOLATION:
		return std::string("The thread tried to read from or write to a virtual\n"
			               "address for which it does not have the appropriate access." );
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return std::string("The thread tried to access an array element that is out\n"
			               "of bounds and the underlying hardware supports bounds checking.");
	case EXCEPTION_BREAKPOINT:
		return std::string("A breakpoint was encountered.");
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return std::string("The thread tried to read or write data that is misaligned\n on hardware\n"
			               "that does not provide alignment. For example, 16-bit values must be aligned\n"
						   "on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.");
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return std::string("One of the operands in a floating-point operation is denormal. A denormal value\n"
			                "is one that is too small to represent as a standard floating-point value.");
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return std::string("The thread tried to divide a floating-point value by a floating-point divisor of zero.");
	case EXCEPTION_FLT_INEXACT_RESULT:
		return std::string("The result of a floating-point operation cannot be represented exactly as a decimal fraction.");
	case EXCEPTION_FLT_INVALID_OPERATION:
		return std::string("This exception represents any floating-point exception not included in this list.");
	case EXCEPTION_FLT_OVERFLOW:
		return std::string("The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.");
	case EXCEPTION_FLT_STACK_CHECK:
		return std::string("The stack overflowed or underflowed as the result of a floating-point operation.");
	case EXCEPTION_FLT_UNDERFLOW:
		return std::string("The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.");
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return std::string("The thread tried to execute an invalid instruction.");
	case EXCEPTION_IN_PAGE_ERROR:
		return std::string("The thread tried to access a page that was not present, and the system was unable\n"
			               "to load the page. For example, this exception might occur if a network connection \n"
						   "is lost while running a program over the network.");
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		return std::string("The thread tried to divide an integer value by an integer divisor of zero.");
	case EXCEPTION_INT_OVERFLOW:
		return std::string("The result of an integer operation caused a carry out of the most significant bit of the result.");
	case EXCEPTION_INVALID_DISPOSITION:
		return std::string("An exception handler returned an invalid disposition to the exception dispatcher. \n"
			               "Programmers using a high-level language such as C should never encounter this exception.");
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		return std::string("The thread tried to continue execution after a noncontinuable exception occurred.");
	case EXCEPTION_PRIV_INSTRUCTION:
		return std::string("The thread tried to execute an instruction whose operation is not allowed in the current machine mode.");
	case EXCEPTION_SINGLE_STEP:
		return std::string("A trace trap or other single-instruction mechanism signaled that one instruction has been executed.");
	case EXCEPTION_STACK_OVERFLOW:
		return std::string("The thread used up its stack.");
	}

	return "UnKnown Exception";
}

#endif

void RegisterExceptionHandler()
{
#ifdef _MSC_VER
	::_set_se_translator(straight_to_debugger);
#endif
}

