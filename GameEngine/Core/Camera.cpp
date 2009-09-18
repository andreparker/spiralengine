#include "Camera.hpp"
#include "../Gfx/Projection.hpp"

using namespace Spiral;

Camera::Camera( const Math::SpMatrix4x4r& proj, const Math::SpMatrix4x4r& view ):
m_view( view ),
m_projection( proj )
{
}

Camera::Camera( const Math::SpMatrix4x4r& proj, const Math::SpVector3r& position ):
m_view( Math::SpMatrix4x4r::Identity() ),
m_projection( proj )
{
	m_view.col( 3 ) = Math::make_vector( position, 1.0f );
}

Camera::Camera( const Math::SpVector3r& position ):
m_view( Math::SpMatrix4x4r::Identity() ),
m_projection( Math::SpMatrix4x4r::Identity() )
{
	m_view.col( 3 ) = Math::make_vector( position, 1.0f );
}

Camera::Camera( const Math::SpMatrix4x4r& proj ):
m_view( Math::SpMatrix4x4r::Identity() ),
m_projection( proj )
{

}

Camera::Camera():
m_view( Math::SpMatrix4x4r::Identity() ),
m_projection( Math::SpMatrix4x4r::Identity() ),
m_bProjDirty(true),
m_bViewDirty(true)
{

}

void Camera::SetProjection( const Projection& projection )
{
	m_projection = projection.proj;
	m_bProjDirty = true;
}

void Camera::GetInverseView( Math::SpMatrix4x4r& view )
{
	view = m_view.transpose();
	view.row( 3 ) = Math::make_row_vector( 0.0f, 0.0f, 0.0f, 1.0f );
	
 	Math::SpVector4r position = m_view.col( 3 );
 	view.col( 3 )[ 0 ] = position.dot( view.row( 0 ) );
 	view.col( 3 )[ 1 ] = position.dot( view.row( 1 ) );
 	view.col( 3 )[ 2 ] = position.dot( view.row( 2 ) );

}

void Camera::RotateAxis( const Math::SpVector3r& axis, SpReal angle )
{
	Math::SpAngleAxisr rotation( Math::Angle2Degree( angle ), axis );
	m_view.block<3,3>(0,0) = m_view.block<3,3>(0,0) * rotation.toRotationMatrix();
	m_bViewDirty = true;
}

void Camera::RotateAroundX( SpReal angle )
{
	RotateAxis( Math::make_vector( 1.0f, 0.0f, 0.0f ), angle );
}

void Camera::RotateAroundY( SpReal angle )
{
	RotateAxis( Math::make_vector( 0.0f, 1.0f, 0.0f ), angle );
}

void Camera::RotateAroundZ( SpReal angle )
{
	RotateAxis( Math::make_vector( 0.0f, 0.0f, 1.0f ), angle );
}

