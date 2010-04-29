#ifndef DIRECT3D9_DRIVER_HPP
#define DIRECT3D9_DRIVER_HPP

#include "../GfxDriver.hpp"
#include <windows.h>
#include <D3D9.h>

namespace Spiral
{
	class Direct3D9Driver : public GfxDriver
	{
	public:
		Direct3D9Driver();
		~Direct3D9Driver();

	private:
		virtual bool DoInitialize( const boost::any& data );
		virtual bool DoUnInitialize();

		virtual void DoSetWorld( const Math::Matrix4x4f& world );
		virtual void DoSetView( const Math::Matrix4x4f& view );
		virtual void DoSetProjection( const Math::Matrix4x4f& proj );
		virtual void DoGetView( Math::Matrix4x4f& view );
		virtual void DoGetProjection( Math::Matrix4x4f& proj );

		virtual void DoGetCaps( GfxDeviceCaps& caps )const;
		virtual void DoDraw( const Math::Vector2f& position, const Rect< SpReal >& rect, const Rect< SpReal >& textureCoords );
		virtual void DoDraw( const std::list< Sprite* >& spriteList );
		virtual void DoDraw( boost::shared_ptr<Sprite>& sprite );
		virtual bool DoCreateSprite( boost::shared_ptr< Sprite >& sprite, boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo );
		virtual void DoSet( const ClearInfoType_t& type, boost::int32_t value );
		virtual void DoSet( const ClearInfoType_t& type, const Rgba& color );
		virtual void DoSet( const ClearInfoType_t& type, SpReal value );
		virtual void DoSet( const BlendMode_t& mode );
		virtual void DoClearBuffer(const BufferInfo_t& buffer);
		virtual void DoSetViewPort( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1 );
		virtual void DoGetViewPort( Rect<boost::int32_t>& viewPort );
		virtual void DoSetClipRect( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1  );
		virtual void DoSetState( const RenderState& state );
		virtual void DoDraw( boost::shared_ptr<Geometry>& geometry );
		virtual bool DoCreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry );
		virtual void DoBind( const boost::shared_ptr< Texture >& texture, boost::int32_t unit );
		virtual bool DoCreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::int8_t* data );
		virtual bool DoSetVideo( const GfxVidInfo_t& info, bool bfullscreen );
		virtual bool DoEnumerateSettings( std::list< GfxVidInfo_t >& modeList );
		virtual bool DoBeginDraw();
		virtual void DoEndDraw();
		virtual bool DoPresent();
	private:
		void InitDefaultPresentSettings( D3DPRESENT_PARAMETERS& presentSettings );
		void InitFullScreenPresentSettings( D3DPRESENT_PARAMETERS& presentSettings, const GfxVidInfo_t& info );
		void InitWindowedPresentSettings( D3DPRESENT_PARAMETERS& presentSettings, const GfxVidInfo_t& info );
		void ReleaseD3DInterfaces();

		bool FindDisplayMode( const GfxVidInfo_t& info, D3DDISPLAYMODE& display );
	private:
		HWND m_focusWindow;
		IDirect3D9* m_d3d9;
		IDirect3DDevice9* m_device9;
	};
}
#endif