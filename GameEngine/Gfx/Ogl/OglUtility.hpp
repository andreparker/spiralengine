/*!
*/
#ifndef OGL_UTILITY_HPP
#define OGL_UTILITY_HPP

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

namespace Spiral
{

	class Geometry;
namespace OglUtil
{
	void EnableArrayPointers( boost::int32_t vertexFormat, boost::int32_t stride, boost::int8_t* data );
	void DisableArrayPointers( boost::int32_t vertexFormat );
	void SetIndexBufferPointer( boost::int8_t* bufferPtr, boost::int32_t indexType );
	void Draw( boost::shared_ptr<Geometry>& geometry );
	boost::int8_t* GetIndexBufferPointer();
	boost::int32_t GetIndexBufferEnum();
}

}
#endif