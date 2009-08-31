/*!
*/
#ifndef OGL_SW_INDEX_BUFFER_HPP
#define OGL_SW_INDEX_BUFFER_HPP

#include "../IndexBuffer.hpp"

namespace Spiral
{
	class OglSWIndexBuffer : public IndexBuffer
	{
	public:
		OglSWIndexBuffer();


	private:
		virtual OglSWIndexBuffer* DoClone()const;
		virtual void DoBind();
		virtual void DoUnBind();
		virtual bool DoCreate( const IndexFormat& format, boost::int32_t indexCount, bool bManaged );


	};

}
#endif