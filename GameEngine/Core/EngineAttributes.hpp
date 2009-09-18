#ifndef ENGINE_ATTRIBUTES_HPP
#define ENGINE_ATTRIBUTES_HPP

namespace Spiral
{

	struct EngineAttribute
	{
		typedef enum
		{
			INVALID_ATTR = -1,
			ClearColorBuffer,
			ClearColor,
			ClearDepthBuffer,
			EnableSprites
		}AttributeType;

		EngineAttribute( AttributeType attr ):
			m_attribute( attr ){}

		AttributeType m_attribute;
	protected:
		EngineAttribute(){}

	};
}
#endif