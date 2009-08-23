#include "RenderQueueException.hpp"

using namespace Spiral;

RenderQueueException::RenderQueueException(const std::string &errorStr) throw():
BaseException( "Gfx:RenderQueue", errorStr )
{
}

const char* RenderQueueException::what()const throw()
{
	std::string& final = GetGeneralError(); 
	return final.c_str();
}