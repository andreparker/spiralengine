#ifndef MATH_HPP
#define MATH_HPP

#include "../Core/Sp_DataTypes.hpp"
#include "../../ThirdParty/Math/Eigen/Core"
#include "../../ThirdParty/Math/Eigen/Geometry"

// creates a operator to align 16
// specifie this if you have a matrix/vector in a data structure
// e.i
// class MyClass
// {
//  public:
//    MAKE_ALIGNED_NEW
//    ... declarations
//  private:
//    SpVector3r position;
//    SpVector2r etc;
//    ....
// }
#define MAKE_ALIGNED_NEW EIGEN_MAKE_ALIGNED_OPERATOR_NEW

namespace Spiral
{

namespace Math
{
	const SpReal PI = 3.141592f;

	typedef Eigen::Matrix< SpReal, 2, 1 > SpVector2r;
	typedef Eigen::Matrix< SpReal, 3, 1 > SpVector3r;
	typedef Eigen::Matrix< SpReal, 4, 1 > SpVector4r;

	typedef Eigen::Matrix< SpReal, 2, 2 > SpMatrix2x2r;
	typedef Eigen::Matrix< SpReal, 3, 3 > SpMatrix3x3r;
	typedef Eigen::Matrix< SpReal, 4, 4 > SpMatrix4x4r;

	typedef Eigen::Transform< SpReal, 2 > SpTransform2r;
	typedef Eigen::Transform< SpReal, 3 > SpTransform3r;
	typedef Eigen::Transform< SpReal, 4 > SpTransform4r;

	typedef Eigen::AngleAxis< SpReal > SpAngleAxisr;


	inline SpReal Angle2Degree( SpReal angle )
	{
		return angle * ( PI / 180.0f );
	}

	inline SpReal Degree2Angle( SpReal degree )
	{
		return degree * ( 180.0f / PI );
	}


	template< class Scalar >
	const Eigen::Matrix< Scalar, 1, 4 > make_row_vector( Scalar c0, Scalar c1, Scalar c2, Scalar c3 )
	{
		return Eigen::Matrix< Scalar, 1, 4 >( c0, c1, c2, c3 );
	}

	template< class Scalar >
	const Eigen::Matrix< Scalar, 2, 1 > make_vector( Scalar c0, Scalar c1 )
	{
		return Eigen::Matrix< Scalar, 2, 1 >( c0, c1 );
	}

	template< class Scalar >
	const Eigen::Matrix< Scalar, 3, 1 > make_vector( Scalar c0, Scalar c1, Scalar c2 )
	{
		return Eigen::Matrix< Scalar, 3, 1 >( c0, c1, c2 );
	}

	template< class Scalar >
	const Eigen::Matrix< Scalar, 4, 1 > make_vector( Scalar c0, Scalar c1, Scalar c2, Scalar c3 )
	{
		return Eigen::Matrix< Scalar, 4, 1 >( c0, c1, c2, c3 );
	}

	template< class Scalar >
	const Eigen::Matrix< Scalar, 4, 1 > make_vector( const Eigen::Matrix< Scalar, 3, 1 >& vec, Scalar c0 )
	{
		return Eigen::Matrix< Scalar, 4, 1 >( vec[0], vec[1], vec[2], c0 );
	}

	bool UnProject( const SpVector3r& winPos, const SpMatrix4x4r& modelViewProj, const Rect<SpReal>& viewPort, SpVector3r& unProjPos );
	bool Project( const SpVector3r& ProjPos,const SpMatrix4x4r& modelViewProj, const Rect<SpReal>& viewPort, SpVector3r& winPos );
}
}


#endif