/*!
*/
#ifndef OGL_DRIVER_HPP
#define OGL_DRIVER_HPP

#include <list>
#include "../GfxDriver.hpp"


namespace Spiral
{

	class OglDriver /*: public GfxDriver*/
	{
	public:
		OglDriver();
		~OglDriver();

		MAKE_ALIGNED_NEW

		bool Initialize( const boost::any& data );
		bool UnInitialize();

		void SetWorld( const Math::SpMatrix4x4r& world );
		void SetView( const Math::SpMatrix4x4r& view );
		void SetProjection( const Math::SpMatrix4x4r& proj );
		void GetView( Math::SpMatrix4x4r& view );
		void GetProjection( Math::SpMatrix4x4r& proj );

		void Draw( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textureCoords );
		void Draw( const std::list< Sprite* >& spriteList );
		void Draw( boost::shared_ptr<Sprite>& sprite );
		bool CreateSprite( boost::shared_ptr< Sprite >& sprite, boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo );
		void Set( const ClearInfoType_t& type, boost::int32_t value );
		void Set( const ClearInfoType_t& type, const Rgba& color );
		void Set( const ClearInfoType_t& type, SpReal value );
		void Set( const BlendMode_t& mode );
		void ClearBuffer(const BufferInfo_t& buffer);
		void SetViewPort( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1 );
		void SetState( const RenderState& state );
		void Draw( boost::shared_ptr<Geometry>& geometry );
		bool CreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry );
		void Bind( const boost::shared_ptr< Texture >& texture, boost::int32_t unit );
		bool CreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::int8_t* data );
		bool SetVideo( const GfxVidInfo_t& info, bool bfullscreen );
		bool EnumerateSettings( std::list< GfxVidInfo_t >& modeList );
		bool BeginDraw();
		void EndDraw();
		bool Present();
	private:
		
		void ProcessState( const RenderState &state );
		void InitializeSpriteVertexBuffer( boost::shared_ptr< VertexBuffer >& buffer, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo );
		void ReCalcModelView();
	private:
		typedef std::list< RenderState* >::iterator StateItr;
		std::list< RenderState* > m_stateList;

		Math::SpMatrix4x4r m_view,m_world;

	};
}

#endif