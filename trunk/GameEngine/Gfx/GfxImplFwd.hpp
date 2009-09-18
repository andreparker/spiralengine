/*!
*/
#ifndef GFX_IMPL_FWD_HPP
#define GFX_IMPL_FWD_HPP

namespace Spiral
{

namespace Impl
{
	struct GfxVidInfo;
	struct TextureInfo;
	struct BufferInfo;
	struct ClearInfoType;
	struct BlendMode;
}

	typedef Impl::TextureInfo TextureInfo_t;
	typedef Impl::GfxVidInfo GfxVidInfo_t;
	typedef Impl::BufferInfo BufferInfo_t;
	typedef Impl::ClearInfoType ClearInfoType_t;
	typedef Impl::BlendMode BlendMode_t;
}
#endif