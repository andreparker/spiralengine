#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>


#include "../../../ThirdParty/GLee/GLee.h"
#include "../../Resource/Resource.hpp"
#include "../../Resource/ResLockImpl.hpp"
#include "../../Core/Rect.hpp"

#include "OglUtility.hpp"
#include "OglDriver.hpp"
#include "../RenderState.hpp"
#include "OglGeometry.hpp"
#include "OglTexture.hpp"
#include "OglSWVertexBuffer.hpp"

#include "../VertexFormatImpl.hpp"
#include "../GeometryType.hpp"
#include "../VertexBuffer.hpp"
#include "../IndexBuffer.hpp"
#include "../GfxImpl.hpp"
#include "../Color.hpp"
#include "../Sprite.hpp"

using namespace Spiral;
using namespace std;
using namespace boost;
using namespace boost::tuples;

OglDriver::OglDriver():
m_stateList(),
m_view(),
m_world( Math::SpMatrix4x4r::Identity() )
{

}

OglDriver::~OglDriver()
{
	BOOST_FOREACH( RenderState* state, m_stateList )
	{
		delete state;
	}

	m_stateList.clear();
}

bool OglDriver::Initialize( const boost::any& /*data*/ )
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	return true;
}

bool OglDriver::UnInitialize()
{
	return true;
}

void OglDriver::ClearBuffer(const BufferInfo_t& buffer)
{
	GLbitfield bits = 0;
	if( buffer.buffer.test( BufferInfo_t::COLOR_BUFFER ) )
	{
		bits |= GL_COLOR_BUFFER_BIT;
	}

	if( buffer.buffer.test( BufferInfo_t::DEPTH_BUFFER ) )
	{
		bits |= GL_DEPTH_BUFFER_BIT;
	}

	if( buffer.buffer.test( BufferInfo_t::STENCIL_BUFFER ) )
	{
		bits |= GL_STENCIL_BUFFER_BIT;
	}

	glClear( bits );
}

void OglDriver::SetViewPort( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1 )
{
	glViewport( (GLint)x0, (GLint)y0, (GLint)x1, (GLint)y1 );
}

void OglDriver::SetState( const RenderState& state )
{
	StateItr itr = find_if( m_stateList.begin(), m_stateList.end(), bind( &RenderState::Cmp, _1, state ) );
	bool dirty = true;

	// check to see if the state is cached
	if( itr != m_stateList.end() )
	{
		if( (*itr)->GetValue() == state.GetValue() )
		{
			dirty = false;
		}else
		{
			*(*itr) = RenderState( state );
		}
		
	}else
	{
		m_stateList.push_back( new RenderState( state ) );
	}

	if( dirty )
	{
		ProcessState(state);
	}
}

void OglDriver::Draw( boost::shared_ptr<Geometry>& geometry )
{
	OglUtil::Draw( geometry );
}

void OglDriver::ProcessState( const RenderState &state )
{
	GLenum cap = static_cast<GLenum>( state.GetState() );
	if( state.GetValue() == RenderState::RS_TRUE )
	{
		glEnable( cap );
	}else if( state.GetValue() == RenderState::RS_FALSE )
	{
		glDisable( cap );
	}
}

bool OglDriver::CreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry )
{
	OglGeometry* newGeometry = new OglGeometry( type );
	geometry.reset( newGeometry );
	return bool( geometry );
}

void OglDriver::Bind( const boost::shared_ptr< Texture >& texture, boost::int32_t unit )
{
	 OglTexture* ogltexture = static_cast< OglTexture* >( texture.get() );
	 ogltexture->Bind( unit );
}

bool OglDriver::CreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::int8_t* data )
{
	OglTexture* newTexture = new OglTexture( info, data );
	texture.reset( newTexture );

	return newTexture->GetResource()->IsValid();
}

bool OglDriver::SetVideo( const GfxVidInfo_t& /*info*/, bool /*bfullscreen*/ )
{
	return true;
}

bool OglDriver::EnumerateSettings( std::list< GfxVidInfo_t >& /*modeList*/ )
{
	return true;
}

bool OglDriver::BeginDraw()
{
	return true;
}

void OglDriver::EndDraw()
{

}

bool OglDriver::Present()
{
	glFlush();
	return true;
}

void OglDriver::Set( const ClearInfoType_t& type, boost::int32_t value )
{
	if( type.type == ClearInfoType_t::StencilValue )
	{
		glClearStencil( static_cast<GLint>( value ) );
	}
}

void OglDriver::Set( const ClearInfoType_t& type, const Rgba& color )
{
	switch ( type.type )
	{
	case ClearInfoType_t::ColorValue:
		glClearColor( color[ Rgba::Red ], color[ Rgba::Green ], color[ Rgba::Blue ], color[ Rgba::Alpha ] );
		break;
	case ClearInfoType_t::AccumValue:
		glClearAccum( color[ Rgba::Red ], color[ Rgba::Green ], color[ Rgba::Blue ], color[ Rgba::Alpha ] );
		break;
	default:
		break;
	}
}

void OglDriver::Set( const ClearInfoType_t& type, SpReal value )
{
	if( type.type == ClearInfoType_t::DepthValue )
	{
		glClearDepth( value );
	}
}

bool OglDriver::CreateSprite( boost::shared_ptr< Sprite >& sprite, boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo )
{
	OglSWVertexBuffer* newVertexBuffer = new OglSWVertexBuffer;
	shared_ptr< VertexBuffer > buffer( newVertexBuffer );
	VertexFormat format = VertexFormat::Create_V3T2();

	if( newVertexBuffer->Create( format, VertexFormat::VFS_V3T2 , 4 ) )
	{
		InitializeSpriteVertexBuffer( buffer, spriteTexCoords, spriteInfo );
		Sprite* newSprite = new Sprite( texture, spriteTexCoords, spriteInfo );
		newSprite->SetVertexBuffer( buffer );
		sprite.reset( newSprite );
		return true;
	}

	return false;
}

void OglDriver::InitializeSpriteVertexBuffer( boost::shared_ptr< VertexBuffer >& buffer, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo )
{
	boost::shared_ptr< Resource > resource = buffer->GetResource();

	ResLockInfo_t lockInfo;
	if( resource->Lock( 0, VertexFormat::VFS_V3T2 * 4, lockInfo ) )
	{
		const int32_t elementCount = VertexFormat::VFS_V3T2 / 4;
		SpReal* data = reinterpret_cast< SpReal* >( lockInfo.data );

		SpReal bottom = spriteInfo.bottom - spriteInfo.top;
		SpReal right  = spriteInfo.right - spriteInfo.left;

		VertexFormat::V3T2_Storage storage;

		/*
			top right
		*/
		storage = make_tuple(right, -spriteInfo.top, -1.0f, spriteTexCoords.right, spriteTexCoords.top );
		VertexFormat::SetV3T2Data( data, storage );
		data += elementCount;

		/*
			top left
		*/
		storage = make_tuple( -spriteInfo.left,-spriteInfo.top, -1.0f, spriteTexCoords.left, spriteTexCoords.top );
		VertexFormat::SetV3T2Data( data, storage );;
		data += elementCount;

		/*
			bottom right
		*/
		storage = make_tuple(right, bottom, -1.0f, spriteTexCoords.right, spriteTexCoords.bottom );
		VertexFormat::SetV3T2Data( data, storage );
		data += elementCount;

		/*
			bottom left
		*/
		storage = make_tuple(-spriteInfo.left, bottom, -1.0f, spriteTexCoords.left, spriteTexCoords.bottom );
		VertexFormat::SetV3T2Data( data, storage );
		data += elementCount;

		resource->Unlock();
	}

}

inline void DrawSprite( const Sprite* sprite, const shared_ptr< VertexBuffer > vertexBuffer )
{
	const Math::SpVector2r& position = sprite->GetPosition();
	glPushMatrix();
	{
		glTranslatef( position[0]+sprite->GetRotOffsetX(), position[1]+sprite->GetRotOffsetY(), 0.0f );
		if( sprite->GetAngle() != 0.0f )
		{
			glRotatef( sprite->GetAngle(), 0.0f, 0.0f, 1.0f );
		}

		OglUtil::Draw( vertexBuffer, GeometryType::GT_TRIANGLE_STRIP );
	}
	glPopMatrix();
}

void OglDriver::Draw( boost::shared_ptr<Sprite>& sprite )
{
	shared_ptr< VertexBuffer > buffer = sprite->GetVertexBuffer();
	Bind( sprite->GetTexture(), 0 );
	DrawSprite( sprite.get(), buffer );
}

void OglDriver::Draw( const std::list< Sprite* >& spriteList )
{
	typedef std::list< Sprite* >::const_iterator Itr;
	shared_ptr< VertexBuffer > vertexBuffer;
	for( Itr itr = spriteList.begin(); itr != spriteList.end(); ++itr )
	{
		vertexBuffer = (*itr)->GetVertexBuffer();
		Bind( (*itr)->GetTexture(), 0 );
		
		// blending
		if( (*itr)->IsAdditive() )
		{
			glBlendFunc( GL_ONE, GL_ONE );
		}else if( (*itr)->hasAlpha() )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}

		DrawSprite( *itr, vertexBuffer );
	}
}

void OglDriver::SetWorld( const Math::SpMatrix4x4r& world )
{
	m_world = world;
	ReCalcModelView();
}

void OglDriver::SetView( const Math::SpMatrix4x4r& view )
{
	m_view = view;
	ReCalcModelView();
}

void OglDriver::ReCalcModelView()
{
	Math::SpMatrix4x4r modelView = m_world * m_view;
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( modelView.data() );
}

void OglDriver::SetProjection( const Math::SpMatrix4x4r& proj )
{
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( proj.data() );
	glMatrixMode( GL_MODELVIEW );
}

void OglDriver::GetView( Math::SpMatrix4x4r& view )
{
	view = m_view;
}

void OglDriver::GetProjection( Math::SpMatrix4x4r& proj )
{
	SpReal projection[16];
	glGetFloatv( GL_PROJECTION_MATRIX, projection );
	proj = Math::SpMatrix4x4r(projection);
}


void OglDriver::Set( const BlendMode_t& mode )
{
	if( mode.GetBlendMode() == BlendMode_t::SrcAlpha_OneMinusSrcAlpha )
	{
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}else if( mode.GetBlendMode() == BlendMode_t::SrcClr_Plus_DstClr )
	{
		glBlendFunc( GL_ONE, GL_ONE );
	}
}

void OglDriver::Draw( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textureCoords )
{
	const int32_t count = 5;
	SpReal elements[ count * 4 ];
	SpReal* data = elements;
	
	VertexFormat::V3T2_Storage storage;

	/*
		top right
	*/
	storage = make_tuple( rect.right, rect.top, -1.0f, textureCoords.right, textureCoords.top );
	VertexFormat::SetV3T2Data( data, storage );
	data += count;

	/*
		top left
	*/
	storage = make_tuple( rect.left, rect.top, -1.0f, textureCoords.left, textureCoords.top );
	VertexFormat::SetV3T2Data( data, storage );;
	data += count;

	/*
		bottom right
	*/
	storage = make_tuple( rect.right, rect.bottom, -1.0f, textureCoords.right, textureCoords.bottom );
	VertexFormat::SetV3T2Data( data, storage );
	data += count;

	/*
		bottom left
	*/
	storage = make_tuple( rect.left, rect.bottom, -1.0f, textureCoords.left, textureCoords.bottom );
	VertexFormat::SetV3T2Data( data, storage );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, count * sizeof(SpReal), elements );
	glTexCoordPointer( 2, GL_FLOAT, count * sizeof(SpReal), elements + 3 );

	glPushMatrix();
	{
		glTranslatef( position[0], position[1], 0.0f );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		glPopMatrix();
	}

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

}