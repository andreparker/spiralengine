#ifndef MATH_HPP
#define MATH_HPP

#include "../Core/Sp_DataTypes.hpp"
#include "../Core/Rect.hpp"
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
//    Vector3f position;
//    Vector2f etc;
//    ....
// }
// NOTE: any class containing a Matrix or vector or containing a class that has a matrix or vector
//       must be allocated on the heap
#define MAKE_ALIGNED_NEW EIGEN_MAKE_ALIGNED_OPERATOR_NEW

namespace Spiral
{

namespace Math
{
	const SpReal PI = 3.141592f;

	typedef Eigen::Matrix< SpReal, 2, 1 > Vector2f;
	typedef Eigen::Matrix< SpReal, 3, 1 > Vector3f;
	typedef Eigen::Matrix< SpReal, 4, 1 > Vector4f;

	typedef Eigen::Matrix< SpReal, 2, 2 > Matrix2x2f;
	typedef Eigen::Matrix< SpReal, 3, 3 > Matrix3x3f;
	typedef Eigen::Matrix< SpReal, 4, 4 > Matrix4x4f;

	typedef Eigen::Transform< SpReal, 2 > Transform2f;
	typedef Eigen::Transform< SpReal, 3 > Transform3f;
	typedef Eigen::Transform< SpReal, 4 > Transform4f;

	typedef Eigen::AngleAxis< SpReal > AngleAxisf;


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

	bool UnProject( const Vector3f& winPos, const Matrix4x4f& modelViewProj, const Rect<SpReal>& viewPort, Vector3f& unProjPos );
	bool Project( const Vector3f& ProjPos,const Matrix4x4f& modelViewProj, const Rect<SpReal>& viewPort, Vector3f& winPos );
}
}


#endif