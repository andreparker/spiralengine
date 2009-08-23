/*!
*/
#ifndef GFX_IMPL_HPP
#define GFX_IMPL_HPP

namespace Spiral
{
namespace Impl
{

	struct GfxVidInfo
	{
		boost::int32_t width, 
			height, 
			bitDepth;
	};

	struct TextureInfo
	{
		boost::int32_t width;
		boost::int32_t height;
		boost::int32_t bitDepth;
		bool           bManaged;
	};

}

typedef Impl::TextureInfo TextureInfo_t;
typedef Impl::GfxVidInfo GfxVideoInfo_t;

}
#endif