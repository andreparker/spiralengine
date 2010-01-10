#ifndef PROJECTION_HPP
#define PROJECTION_HPP

#include "../Math/Math.hpp"
#include "../Core/Sp_DataTypes.hpp"


namespace Spiral
{
	struct Projection
	{
		Math::Matrix4x4f proj;
		/*!
		   @function  CreateOrtho2D
		   @brief     creates a 2d view
		   @return    Spiral::Projection
		   @param     SpReal left
		   @param     SpReal top
		   @param     SpReal right
		   @param     SpReal bottom
		*/
		static Projection CreateOrtho2D( SpReal left, SpReal top, SpReal right, SpReal bottom );

		/*!
		   @function  CreatePerspective
		   @brief     creates a perspective view
		   @return    Spiral::Projection
		   @param     SpReal fov - angle in radians
		   @param     SpReal aspectRatio
		   @param     SpReal zNear
		   @param     SpReal zFar
		*/
		static Projection CreatePerspective( SpReal fov, SpReal aspectRatio, SpReal zNear, SpReal zFar );
	};
}

#endif