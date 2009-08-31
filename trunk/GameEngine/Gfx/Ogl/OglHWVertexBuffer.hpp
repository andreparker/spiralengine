/*!
*/
#ifndef OGL_HW_VERTEX_BUFFER_HPP
#define OGL_HW_VERTEX_BUFFER_HPP

#include "../VertexBuffer.hpp"

namespace Spiral
{
	class OglHWVertexBuffer : public VertexBuffer
	{
	public:
		OglHWVertexBuffer();


	private:
		virtual OglHWVertexBuffer* DoClone()const;
		virtual void DoBind();
		virtual void DoUnBind();
		virtual bool DoCreate( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged );

	};

}
#endif