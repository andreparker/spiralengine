#include <cmath>
#include "Projection.hpp"

using namespace Spiral;
USING_PART_OF_NAMESPACE_EIGEN

Projection Projection::CreateOrtho2D( SpReal left, SpReal top, SpReal right, SpReal bottom )
{
	Projection projection;
	const SpReal zNear = 1.0f;
	const SpReal zFar = -1.0f;

	projection.proj = Math::SpMatrix4x4r::Identity();
	projection.proj( 0, 0 ) = 2.0f / ( right - left );
	projection.proj( 1, 1 ) = 2.0f / ( top - bottom );
	projection.proj( 2, 2 ) = -2.0f / ( zFar - zNear );

	projection.proj( 0, 3 ) = -( right + left )/( right - left );
	projection.proj( 1, 3 ) = -( top + bottom )/( top - bottom );
	projection.proj( 2, 3 ) = -( zFar + zNear )/( zFar - zNear );
	projection.proj( 3, 3 ) = 1.0f;

	return projection;
}

Projection Projection::CreatePerspective( SpReal fov, SpReal aspectRatio, SpReal zNear, SpReal zFar )
{
	Projection projection;

	projection.proj = Math::SpMatrix4x4r::Identity();
	projection.proj( 0, 0 ) = aspectRatio;
	projection.proj( 1, 1 ) = 1.0f / tan( Math::Angle2Degree( fov / 2.0f )  );
	projection.proj( 2, 2 ) = ( zFar + zNear )/( zNear - zFar );

	projection.proj( 3, 2 ) = -1.0f;
	projection.proj( 2, 3 ) = 2.0f * ( zFar * zNear ) / ( zNear - zFar );
	projection.proj( 3, 3 ) = 0;

	return projection;
}