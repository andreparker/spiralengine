#include "Camera.hpp"
#include "../Gfx/Projection.hpp"

using namespace Spiral;

Camera::Camera( const Math::Matrix4x4f& proj, const Math::Matrix4x4f& view ):
m_view( view ),
m_projection( proj )
{
}

Camera::Camera( const Math::Matrix4x4f& proj, const Math::Vector3f& position ):
m_view( Math::Matrix4x4f::Identity() ),
m_projection( proj )
{
	m_view.col( 3 ) = Math::make_vector( position, 1.0f );
}

Camera::Camera( const Math::Vector3f& position ):
m_view( Math::Matrix4x4f::Identity() ),
m_projection( Math::Matrix4x4f::Identity() )
{
	m_view.col( 3 ) = Math::make_vector( position, 1.0f );
}

Camera::Camera( const Math::Matrix4x4f& proj ):
m_view( Math::Matrix4x4f::Identity() ),
m_projection( proj )
{

}

Camera::Camera():
m_view( Math::Matrix4x4f::Identity() ),
m_projection( Math::Matrix4x4f::Identity() ),
m_bProjDirty(true),
m_bViewDirty(true)
{

}

void Camera::SetProjection( const Projection& projection )
{
	m_projection = projection.proj;
	m_bProjDirty = true;
}

void Camera::GetInverseView( Math::Matrix4x4f& view )
{
	view = m_view.transpose();
	view.row( 3 ) = Math::make_row_vector( 0.0f, 0.0f, 0.0f, 1.0f );
	
 	Math::Vector4f position = m_view.col( 3 );
 	view.col( 3 )[ 0 ] = position.dot( view.row( 0 ) );
 	view.col( 3 )[ 1 ] = position.dot( view.row( 1 ) );
 	view.col( 3 )[ 2 ] = position.dot( view.row( 2 ) );

}

void Camera::RotateAxis( const Math::Vector3f& axis, SpReal angle )
{
	Math::AngleAxisf rotation( Math::Angle2Degree( angle ), axis );
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

