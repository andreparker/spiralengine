/*!
*/
#ifndef OGL_SW_VERTEX_BUFFER_HPP
#define OGL_SW_VERTEX_BUFFER_HPP

#include "../VertexBuffer.hpp"

namespace Spiral
{
	class OglSWVertexBuffer : public VertexBuffer
	{
	public:
		OglSWVertexBuffer();
		

	private:
		virtual OglSWVertexBuffer* DoClone()const;
		virtual void DoBind();
		virtual void DoUnBind();
		virtual bool DoCreate( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged );

	};

}
#endif