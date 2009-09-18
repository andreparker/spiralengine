/*!
*/
#ifndef COLOR_HPP
#define COLOR_HPP

#include <boost/cstdint.hpp>
#include "../Core/Sp_DataTypes.hpp"

#define ARGB_DWORD( a, r, g, b ) (((a)&0xff) << 24)|(((r)&0xff) << 16)|(((g)&0xff) << 8)|((b)&0xff)
#define BGRA_DWORD( b, g, r, a ) ARGB_DWORD( b, g, r, a )
#define ABGR_DWORD( a, b, g, r ) ARGB_DWORD( a, b, g, r )

namespace Spiral
{

	class Rgba
	{
	public:

		enum
		{
			Red,
			Green,
			Blue,
			Alpha
		};

		Rgba( SpReal red,SpReal green,SpReal blue,SpReal alpha = 1.0f ):
		  m_r( red ),m_g( green ),m_b( blue ),m_a( alpha )
		  {

		  }

		Rgba():m_r(0),m_g(0),m_b(0),m_a(1.0f){}


		const SpReal operator[]( boost::int32_t idx )const
		{
			return color[ idx ];
		}

		SpReal& operator[]( boost::int32_t idx )
		{
			return color[ idx ];
		}

		operator SpReal*()
		{
			 return color;
		}

	private:

		union
		{
			SpReal color[4];
			struct 
			{
				SpReal m_r,m_g,m_b,m_a;
			};
		};

	};

	
	
	inline const Rgba operator*( const Rgba& rgba,SpReal scalar  )
	{
		return Rgba( rgba[0] * scalar, rgba[1] * scalar, rgba[2] * scalar, rgba[3] * scalar );
	}
	
	inline const Rgba operator*( SpReal scalar,const Rgba& rgba  )
	{
		return rgba * scalar;
	}

	inline const Rgba operator*( const Rgba& lhs, const Rgba& rhs )
	{
		return Rgba( lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2], lhs[3] * rhs[3] );
	}

	inline const Rgba operator-( const Rgba& lhs, const Rgba& rhs )
	{
		return Rgba( lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] );
	}

	inline const Rgba operator+( const Rgba& lhs, const Rgba& rhs )
	{
		return Rgba( lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] );
	}

	inline const Rgba ColorLerp( const Rgba& lhs, const Rgba& rhs, SpReal scalar )
	{
		return lhs + ( rhs - lhs ) * scalar;
	}
}

#endif