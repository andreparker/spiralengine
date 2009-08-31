#include "GfxDriver.hpp"
#include "GfxImpl.hpp"
#include "Texture.hpp"

using namespace Spiral;
using namespace boost;

bool GfxDriver::CreateTexture(const TextureInfo_t &info, shared_ptr<Texture> &texture, const uint8_t *data /*= 0*/)
{
	return DoCreateTexture( info, texture, data );
}

bool GfxDriver::SetVideo( const GfxVidInfo_t& info, bool bfullscreen /*= true*/ )
{
	return DoSetVideo( info, bfullscreen );
}

bool GfxDriver::DoEnumerateSettings( std::list< GfxVidInfo_t >& modeList )
{
	return DoEnumerateSettings( modeList );
}

bool GfxDriver::BeginDraw()
{
	return DoBeginDraw();
}

void GfxDriver::EndDraw()
{
	DoEndDraw();
}

bool GfxDriver::Present()
{
	return DoPresent();
}

void GfxDriver::Bind( const shared_ptr< Texture >& texture, int32_t unit /*= 0*/ )
{
	DoBind( texture, unit );
}

bool GfxDriver::CreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry )
{
	return DoCreateGeometry( type, geometry );
}

void GfxDriver::Draw( boost::shared_ptr<Geometry>& geometry )
{
	DoDraw( geometry );
}

void GfxDriver::SetState( const RenderState& state )
{
	DoSetState( state );
}