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
		Rgba( real red,real green,real blue,real alpha = 1.0f ):
		  m_r( red ),m_g( green ),m_b( blue ),m_a( alpha )
		  {

		  }

		Rgba():m_r(0),m_g(0),m_b(0),m_a(1.0f){}


		const real operator[]( boost::int32_t idx )const
		{
			return color[ idx ];
		}

		real& operator[]( boost::int32_t idx )
		{
			return color[ idx ];
		}

		operator real*()
		{
			 return color;
		}

	private:

		union
		{
			real color[4];
			struct 
			{
				real m_r,m_g,m_b,m_a;
			};
		};

	};

	
	
	const Rgba operator*( const Rgba& rgba,real scalar  )
	{
		return Rgba( rgba[0] * scalar, rgba[1] * scalar, rgba[2] * scalar, rgba[3] * scalar );
	}
	
	const Rgba operator*( real scalar,const Rgba& rgba  )
	{
		return rgba * scalar;
	}

	const Rgba operator*( const Rgba& lhs, const Rgba& rhs )
	{
		return Rgba( lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2], lhs[3] * rhs[3] );
	}

	const Rgba operator-( const Rgba& lhs, const Rgba& rhs )
	{
		return Rgba( lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] );
	}

	const Rgba operator+( const Rgba& lhs, const Rgba& rhs )
	{
		return Rgba( lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] );
	}

	const Rgba ColorLerp( const Rgba& lhs, const Rgba& rhs, real scalar )
	{
		return lhs + ( rhs - lhs ) * scalar;
	}
}

#endif