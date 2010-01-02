/*!
*/
#ifndef RENDER_STATE_HPP
#define RENDER_STATE_HPP

#include <boost/cstdint.hpp>

namespace Spiral
{
	struct RenderState
	{
		RenderState():state( RS_INVALID ),value( 0 ){}

		enum
		{
			RS_INVALID = -1,
			RS_CULL_FACE = 0x0B44,
			RS_DEPTH_TEST = 0x0B71,
			RS_TEXTURE = 0x0DE1,
			RS_BLEND = 0x0BE2,
			RS_CLIP_RECT = 0x0C11
		};

		typedef enum
		{
			RS_FALSE = 0,
			RS_TRUE = 1
		}rs_boolean;

		static RenderState Cull_Face( boost::int32_t value );
		static RenderState Depth_Test( boost::int32_t value );
		static RenderState Texture( boost::int32_t value );
		static RenderState Blend( boost::int32_t value );
		static RenderState ClipRect( boost::int32_t value );

		void SetValue( boost::int32_t val );
		boost::int32_t GetValue()const;
		boost::int32_t GetState()const;

		bool Cmp( const RenderState& state_ )const;
		bool operator==( const RenderState& state_ )const;

	private:
		
		explicit RenderState( boost::int32_t state_, boost::int32_t value_ ):
		state( state_ ), value( value_ ){}

		boost::int32_t state;
		boost::int32_t value;
	};

	inline RenderState RenderState::Blend( boost::int32_t value )
	{
		return RenderState( RS_BLEND, value );
	}

	inline RenderState RenderState::Texture( boost::int32_t value )
	{
		return RenderState( RS_TEXTURE, value );
	}

	inline RenderState RenderState::Cull_Face( boost::int32_t value )
	{
		return RenderState( RS_CULL_FACE, value );
	}

	inline RenderState RenderState::Depth_Test( boost::int32_t value )
	{
		return RenderState( RS_DEPTH_TEST, value );
	}

	inline RenderState RenderState::ClipRect( boost::int32_t value )
	{
		return RenderState( RS_CLIP_RECT, value );
	}

//=======================================================================================
	inline void RenderState::SetValue( boost::int32_t val )
	{
		value = val;
	}

	inline boost::int32_t RenderState::GetValue()const
	{
		return value;
	}

	inline boost::int32_t RenderState::GetState() const
	{
		return state;
	}

	inline bool RenderState::Cmp( const RenderState& state_ )const
	{
		return bool( *this == state_ );
	}

	inline bool RenderState::operator==( const RenderState& state_ )const
	{
		return bool( state == state_.state );
	}
}
#endif