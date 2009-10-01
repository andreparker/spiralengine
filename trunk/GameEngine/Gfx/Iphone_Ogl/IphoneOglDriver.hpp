#ifndef IPHONE_OGL_DRIVER_HPP
#define IPHONE_OGL_DRIVER_HPP

#include "../GfxDriver.hpp"

// glES render context
@class EAGLContext;

// layer class for iphone
@class CAEAGLLayer;

namespace Spiral
{

	class IphoneOglDriver : GfxDriver
	{
	public:
		IphoneOglDriver();
		~IphoneOglDriver();
		
	private:
		virtual bool DoInitialize( const boost::any& data );
        virtual bool DoUnInitialize();

		virtual void DoSetWorld( const Math::SpMatrix4x4r& world );
		virtual void DoSetView( const Math::SpMatrix4x4r& view );
		virtual void DoSetProjection( const Math::SpMatrix4x4r& proj );
		virtual void DoGetView( Math::SpMatrix4x4r& view );
		virtual void DoGetProjection( Math::SpMatrix4x4r& proj );

		virtual void DoDraw( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textureCoords );
		virtual void DoDraw( const std::list< Sprite* >& spriteList );
		virtual void DoDraw( boost::shared_ptr<Sprite>& sprite );
		virtual bool DoCreateSprite( boost::shared_ptr< Sprite >& sprite, boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo );
		virtual void DoSet( const ClearInfoType_t& type, boost::int32_t value );
		virtual void DoSet( const ClearInfoType_t& type, const Rgba& color );
		virtual void DoSet( const ClearInfoType_t& type, SpReal value );
		virtual void DoSet( const BlendMode_t& mode );
		virtual void DoClearBuffer(const BufferInfo_t& buffer);
		virtual void DoSetViewPort( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1 );
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
		
		EAGLContext *m_renderContext;   ///< context to render to 
		CAEAGLLayer* m_layer;			///< needed for render/frame buffer creation
	};
}

#endif