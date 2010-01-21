/*!
*/
#ifndef GFX_IMPL_HPP
#define GFX_IMPL_HPP

#include <bitset>
#include <boost/cstdint.hpp>

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

	struct BufferInfo
	{
		enum
		{
			COLOR_BUFFER = 0,
			DEPTH_BUFFER,
			STENCIL_BUFFER
		};

		BufferInfo( boost::uint32_t bits ):
			buffer( bits ){}
		BufferInfo():
#if defined(_MSC_VER) || defined(_HAS_CPP0X)
			buffer( (_ULonglong)0 ){}
#else
			buffer( (boost::uint32_t)0 ){}
#endif

		std::bitset<8> buffer;
	};

	struct ClearInfoType
	{
		typedef enum
		{
			DepthValue = 1,
			ColorValue,
			StencilValue,
			AccumValue
		}ClearType;

		ClearInfoType( ClearType value ):
			type( value ){}

		ClearType type;
	private:
		ClearInfoType(){}
	};

	struct BlendMode
	{
		typedef enum
		{
			Invalid_Mode = -1,
			SrcAlpha_OneMinusSrcAlpha = 1,
			SrcClr_Plus_DstClr
		}BlendType;

		static BlendMode Additive();
		static BlendMode SrcAlpha();
		BlendType GetBlendMode()const
		{
			return m_blend;
		}
		
		BlendMode():m_blend(Invalid_Mode){}
	protected:
		BlendMode( BlendType b ):
			 m_blend( b ){}	

		BlendType m_blend;
	};

	inline BlendMode BlendMode::Additive()
	{
		return BlendMode( SrcClr_Plus_DstClr );
	}

	inline BlendMode BlendMode::SrcAlpha()
	{
		return BlendMode( SrcAlpha_OneMinusSrcAlpha );
	}
}

typedef Impl::TextureInfo TextureInfo_t;
typedef Impl::GfxVidInfo GfxVideoInfo_t;
typedef Impl::BufferInfo BufferInfo_t;
typedef Impl::ClearInfoType ClearInfoType_t;
typedef Impl::BlendMode BlendMode_t;

}
#endif