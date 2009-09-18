#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Math/Math.hpp"

namespace Spiral
{

	struct Projection;
	class Camera
	{
	public:
		Camera( const Math::SpMatrix4x4r& proj, const Math::SpMatrix4x4r& view );
		Camera( const Math::SpMatrix4x4r& proj, const Math::SpVector3r& position );
		Camera( const Math::SpVector3r& position );
		Camera( const Math::SpMatrix4x4r& proj );
		Camera();

		void SetProjection( const Projection& projection );

		void SetProjection( const Math::SpMatrix4x4r& proj )
		{
			m_projection = proj;
		}

		void SetView( const Math::SpMatrix4x4r& view )
		{
			m_view = view;
		}

		void GetProjection( Math::SpMatrix4x4r& proj )
		{
			proj = m_projection;
		}

		void GetView( Math::SpMatrix4x4r& view )
		{
			view = m_view;
		}
		void GetInverseView( Math::SpMatrix4x4r& view );

		void RotateAxis( const Math::SpVector3r& axis, SpReal angle );
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
		Math::SpMatrix4x4r m_view,m_projection;
		bool m_bProjDirty,m_bViewDirty;

	};
}

#endif