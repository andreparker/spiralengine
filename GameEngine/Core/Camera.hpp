#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Math/Math.hpp"

namespace Spiral
{

	struct Projection;
	class Camera
	{
	public:
		Camera( const Math::Matrix4x4f& proj, const Math::Matrix4x4f& view );
		Camera( const Math::Matrix4x4f& proj, const Math::Vector3f& position );
		Camera( const Math::Vector3f& position );
		Camera( const Math::Matrix4x4f& proj );
		Camera();

		void SetProjection( const Projection& projection );

		void SetProjection( const Math::Matrix4x4f& proj )
		{
			m_projection = proj;
		}

		void SetView( const Math::Matrix4x4f& view )
		{
			m_view = view;
		}

		void GetProjection( Math::Matrix4x4f& proj )
		{
			proj = m_projection;
		}

		void GetView( Math::Matrix4x4f& view )
		{
			view = m_view;
		}
		void GetInverseView( Math::Matrix4x4f& view );

		void RotateAxis( const Math::Vector3f& axis, SpReal angle );
		void RotateAroundX( SpReal angle );
		void RotateAroundY( SpReal angle );
		void RotateAroundZ( SpReal angle );

		bool IsProjectionDirty()const
		{
			return m_bProjDirty;
		}

		bool IsViewDirty()const
		{
			return m_bViewDirty;
		}

		void ResetDirtyFlags()
		{
			m_bProjDirty = m_bViewDirty = false;
		}

		MAKE_ALIGNED_NEW
	private:
		Math::Matrix4x4f m_view,m_projection;
		bool m_bProjDirty,m_bViewDirty;

	};
}

#endif