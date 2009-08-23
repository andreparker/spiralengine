/*!
*/
#ifndef RENDER_QUEUE_EXCEPTION_HPP
#define RENDER_QUEUE_EXCEPTION_HPP

#include "../Core/Sp_Exception.hpp"

namespace Spiral
{
	class RenderQueueException : public BaseException
	{
	public:
		RenderQueueException( const std::string& errorStr )throw();

		virtual const char* what()const throw();
	};
}

#endif