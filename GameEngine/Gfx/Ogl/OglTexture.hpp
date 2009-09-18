#ifndef OGL_TEXTURE_HPP
#define OGL_TEXTURE_HPP

#include "../GfxImplFwd.hpp"
#include "../Texture.hpp"

namespace Spiral
{

	class OglTexture : public Texture
	{
	public:
		OglTexture( const TextureInfo_t& info , const boost::int8_t* data );
		virtual ~OglTexture();

		void Bind( boost::int32_t unit )const;
	private:
		/*!
			@function   DoClone
			@brief  	  returns a copy
			@return     Cloneable*
		*/
		virtual Cloneable* DoClone()const;

	};
}

#endif