/*!
*/
#ifndef GFX_DRIVER_HPP
#define GFX_DRIVER_HPP

#include <list>
#include <boost/cstdint.hpp>

#include "GfxImplFwd.hpp"
#include "GeometryTypeFwd.hpp"
#include "../Core/DeviceDriver.hpp"
#include "../Core/Sp_DataTypes.hpp"
#include "../Core/RectFwd.hpp"

#include "../Math/Math.hpp"


namespace Spiral
{

	class Texture;
	class Geometry;
	class Rgba;
	struct RenderState;
	class Sprite;

	class GfxDriver : public DeviceDriver
	{
	public:

		/*!
		   @function  SetWorld
		   @brief     sets the world transform
		   @return    void
		   @param     const Math::SpMatrix4x4r world
		*/
		void SetWorld( const Math::SpMatrix4x4r& world );

		/*!
		   @function  SetView
		   @brief     sets the view transform
		   @return    void
		   @param     const Math::SpMatrix4x4r view
		*/
		void SetView( const Math::SpMatrix4x4r& view );

		/*!
		   @function  SetProjection
		   @brief     sets the world projection
		   @return    void
		   @param     const Math::SpMatrix4x4r proj
		*/
		void SetProjection( const Math::SpMatrix4x4r& proj );

		/*!
		   @function  GetView
		   @brief     gets the view matrix
		   @return    void
		   @param     Math::SpMatrix4x4r & view
		*/
		void GetView( Math::SpMatrix4x4r& view );

		/*!
		   @function  GetProjection
		   @brief     returns the projection matrix
		   @return    void
		   @param     Math::SpMatrix4x4r & proj
		*/
		void GetProjection( Math::SpMatrix4x4r& proj );

		/*!
		   @function  CreateSprite
		   @brief     creates a 2d sprite
		   @return    bool
		   @param     boost::shared_ptr< Sprite > & sprite
		   @param     boost::shared_ptr< Texture > & texture
		   @param     const Rect< SpReal > & spriteTexCoords
		   @param     const Rect< SpReal > & spriteInfo
		*/
		bool CreateSprite( boost::shared_ptr< Sprite >& sprite, boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo );

		/*!
		   @function  Set
		   @brief     sets the clear values for stencil buffer
		   @return    void
		   @param     const ClearInfoType_t & type
		   @param     boost::int32_t value
		*/
		void Set( const ClearInfoType_t& type, boost::int32_t value );

		/*!
		   @function  Set
		   @brief     sets clear values for color buffer, accum buffer
		   @return    void
		   @param     const ClearInfoType_t & type
		   @param     const Rgba & color
		*/
		void Set( const ClearInfoType_t& type, const Rgba& color );

		/*!
		   @function  Set
		   @brief     sets clear values for depth buffer
		   @return    void
		   @param     const ClearInfoType_t & type
		   @param     SpReal value
		*/
		void Set( const ClearInfoType_t& type, SpReal value );

		/*!
		   @function  Set
		   @brief     sets the blending mode
		   @return    void
		   @param     const BlendMode_t & mode
		*/
		void Set( const BlendMode_t& mode );

		/*!
		   @function  ClearBuffer
		   @brief     clears buffers specified in bufferinfo
		   @return    void
		   @param     const BufferInfo_t & buffer
		*/
		void ClearBuffer(const BufferInfo_t& buffer);

		/*!
		   @function  SetViewPort
		   @brief     sets the viewport
		   @return    void
		   @param     boost::int32_t x0
		   @param     boost::int32_t y0
		   @param     boost::int32_t x1
		   @param     boost::int32_t y1
		*/
		void SetViewPort( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1 );

		/*!
		   @function  SetState
		   @brief     Sets a render state
		   @return    void
		   @param     const RenderState & state - the render state to set
		*/
		void SetState( const RenderState& state );

		/*!
		   @function  Draw
		   @brief     draws a geometry
		   @return    void
		   @param     boost::shared_ptr<Geometry> & geometry
		*/
		void Draw( boost::shared_ptr<Geometry>& geometry );

		/*!
		   @function  Draw
		   @brief     draws a sprite 2d image
		   @return    void
		   @param     boost::shared_ptr<Sprite> & sprite
		*/
		void Draw( boost::shared_ptr<Sprite>& sprite );
		void Draw( const std::list< Sprite* >& spriteList );

		/*!
		   @function  CreateGeometry
		   @brief     creates a geometry
		   @return    bool
		   @param     const GeometryType & type - type of geometry ( e.g TRIANGLES etc)
		   @param     boost::shared_ptr<Geometry> & geometry
		*/
		bool CreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry );

		/*!
			@function   CreateTexture
			@brief  	creates a texture and uploads image data if provided
			@return     bool
			@param 	    const TextureInfo & info
			@param 	    boost::shared_ptr< Texture > & texture
			@param 	    boost::uint8_t * data
		*/
		bool CreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::int8_t* data = NULL );

		/*!
			@function    SetVideo
			@brief       sets the video mode
			@return      bool
			@param       const GfxVidInfo_t & info
			@param       bool bfullscreen - true, sets full screen mode false, tests 
		*/
		bool SetVideo( const GfxVidInfo_t& info, bool bfullscreen = true );

		/*!
			@function    EnumerateSettings
			@brief       finds all valid video modes
			@return      bool
			@param       std::list< GfxVidInfo_t > & modeList
		*/
		bool EnumerateSettings( std::list< GfxVidInfo_t >& modeList );

		/*!
			@function    BeginDraw
			@brief       tells the driver drawing commands are being sent
			@return      bool
		*/
		bool BeginDraw();

		/*!
			@function    EndDraw
			@brief       tells the driver that drawing has stopped
			@return      void
		*/
		void EndDraw();

		/*!
			@function    Present
			@brief       flushes and draws final image to screen/buffer
			@return      bool
		*/
		bool Present();

		/*!
			@function   Bind
			@brief  	  binds a texture to use
			@return     void
			@param 	  const boost::shared_ptr< Texture > & texture
			@param 	  boost::int32_t unit
		*/
		void Bind( const boost::shared_ptr< Texture >& texture, boost::int32_t unit = 0 );


	protected:
		GfxDriver(){}

	private:

       /*!
          @function   DoCreateTexture
          @brief  	  creates a texture and uploads image data if provided
          @return     bool
          @param 	  const TextureInfo_t & info
          @param 	  boost::shared_ptr< Texture > & texture
          @param 	  boost::uint8_t * data
       */
       virtual bool DoCreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::int8_t* data ) = 0;

       /*!
          @function    DoSetVideo
          @brief       sets the video mode
          @return      bool
          @param       const GfxVidInfo_t & info
          @param       bool bfullscreen - true, sets full screen mode false, tests 
       */
       virtual bool DoSetVideo( const GfxVidInfo_t& info, bool bfullscreen = true ) = 0;

       /*!
          @function    DoEnumerateSettings
          @brief       finds all valid video modes
          @return      bool
          @param       std::list< GfxVidInfo_t > & modeList
       */
       virtual bool DoEnumerateSettings( std::list< GfxVidInfo_t >& modeList ) = 0;

       /*!
          @function    DoBeginDraw
          @brief       tells the driver drawing commands are being sent
          @return      bool
       */
       virtual bool DoBeginDraw() = 0;

       /*!
          @function    DoEndDraw
          @brief       tells the driver that drawing has stopped
          @return      void
       */
       virtual void DoEndDraw() = 0;

       /*!
          @function    DoPresent
          @brief       flushes and draws final image to screen/buffer
          @return      bool
       */
       virtual bool DoPresent() = 0;

       /*!
          @function   DoBind
          @brief  	  binds a texture to use
          @return     void
          @param 	  const boost::shared_ptr< Texture > & texture
          @param 	  boost::int32_t unit
       */
       virtual void DoBind( const boost::shared_ptr< Texture >& texture, boost::int32_t unit ) = 0;

	   /*!
	      @function  DoCreateGeometry
	      @brief     
	      @return    bool
	      @param     const GeometryType & type
	      @param     boost::shared_ptr<Geometry> & geometry
	   */
	   virtual bool DoCreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry ) = 0;

	   /*!
	      @function  DoDraw
	      @brief     draw geometry
	      @return    void
	      @param     boost::shared_ptr<Geometry> & geometry
	   */
	   virtual void DoDraw( boost::shared_ptr<Geometry>& geometry ) = 0;
	   virtual void DoDraw( boost::shared_ptr<Sprite>& sprite ) = 0;
	   virtual void DoDraw( const std::list< Sprite* >& spriteList ) = 0;

	   /*!
	      @function  DoSetState
	      @brief     
	      @return    void
	      @param     const RenderState & state
	   */
	   virtual void DoSetState( const RenderState& state ) = 0;

	   virtual void DoSetWorld( const Math::SpMatrix4x4r& world ) = 0;
	   virtual void DoSetView( const Math::SpMatrix4x4r& view ) = 0;
	   virtual void DoSetProjection( const Math::SpMatrix4x4r& proj ) = 0;
	   virtual void DoGetView( Math::SpMatrix4x4r& view ) = 0;
	   virtual void DoGetProjection( Math::SpMatrix4x4r& proj ) = 0;

	   virtual void DoSetViewPort( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1 ) = 0;
	   virtual void DoClearBuffer(const BufferInfo_t& buffer) = 0;

	   virtual void DoSet( const ClearInfoType_t& type, boost::int32_t value ) = 0;
	   virtual void DoSet( const ClearInfoType_t& type, const Rgba& color ) = 0;
	   virtual void DoSet( const ClearInfoType_t& type, SpReal value ) = 0;
	   virtual void DoSet( const BlendMode_t& mode ) = 0;

	   virtual bool DoCreateSprite( boost::shared_ptr< Sprite >& sprite, boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo ) = 0;

	};
}

#endif