#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include "../../../ThirdParty/GLee/GLee.h"
#include "OglUtility.hpp"
#include "OglDriver.hpp"
#include "../RenderState.hpp"
#include "OglGeometry.hpp"
#include "../GeometryType.hpp"
#include "../VertexBuffer.hpp"
#include "../IndexBuffer.hpp"

using namespace Spiral;
using namespace std;
using namespace boost;

OglDriver::OglDriver():
m_stateList()
{

}

OglDriver::~OglDriver()
{
	BOOST_FOREACH( RenderState* state, m_stateList )
	{
		delete state;
	}
}

bool OglDriver::DoInitialize( const boost::any& /*data*/ )
{
	return true;
}

bool OglDriver::DoUnInitialize()
{
	return true;
}

void OglDriver::DoSetState( const RenderState& state )
{
	StateItr itr = find_if( m_stateList.begin(), m_stateList.end(), bind( &RenderState::Cmp, _1, state ) );
	bool dirty = true;
	if( itr != m_stateList.end() )
	{
		if( (*itr)->GetValue() == state.GetValue() )
		{
			dirty = false;
		}

		*(*itr) = RenderState( state );
	}else
	{
		m_stateList.push_back( new RenderState( state ) );
	}

	if( dirty )
	{
		ProcessState(state);
	}
}

void OglDriver::DoDraw( boost::shared_ptr<Geometry>& geometry )
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

bool OglDriver::DoCreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry )
{
	OglGeometry* newGeometry = new OglGeometry( type );
	geometry.reset( newGeometry );
	return bool( geometry );
}

void OglDriver::DoBind( const boost::shared_ptr< Texture >& /*texture*/, boost::int32_t unit )
{
	 glActiveTextureARB( GL_TEXTURE0_ARB + unit );
}

bool OglDriver::DoCreateTexture( const TextureInfo_t& /*info*/, boost::shared_ptr< Texture >& /*texture*/, const boost::uint8_t* /*data*/ )
{
	return true;
}

bool OglDriver::DoSetVideo( const GfxVidInfo_t& /*info*/, bool /*bfullscreen*/ )
{
	return true;
}

bool OglDriver::DoEnumerateSettings( std::list< GfxVidInfo_t >& /*modeList*/ )
{
	return true;
}

bool OglDriver::DoBeginDraw()
{
	return true;
}

void OglDriver::DoEndDraw()
{

}

bool OglDriver::DoPresent()
{
	glFlush();
	return true;
}