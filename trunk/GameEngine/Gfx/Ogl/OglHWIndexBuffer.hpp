/*!
*/
#ifndef OGL_HW_INDEX_BUFFER_HPP
#define OGL_HW_INDEX_BUFFER_HPP

#include "../IndexBuffer.hpp"

namespace Spiral
{

	class OglHWIndexBuffer : public IndexBuffer
	{
	public:
		OglHWIndexBuffer();

	private:
		virtual void DoBind();
		virtual void DoUnBind();
		virtual OglHWIndexBuffer* DoClone()const;
		virtual bool DoCreate( const IndexFormat& format, boost::int32_t indexCount, bool bManaged );
	};
}

#endif