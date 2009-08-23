/*!
*/
#ifndef GFX_DRIVER_HPP
#define GFX_DRIVER_HPP

#include <list>
#include <boost/cstdint.hpp>

#include "GfxImplFwd.hpp"
#include "../Core/DeviceDriver.hpp"


namespace Spiral
{

	class Texture;
	class GfxDriver : public DeviceDriver
	{
	public:

		/*!
			@function   CreateTexture
			@brief  	creates a texture and uploads image data if provided
			@return     bool
			@param 	    const TextureInfo & info
			@param 	    boost::shared_ptr< Texture > & texture
			@param 	    boost::uint8_t * data
		*/
		bool CreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::uint8_t* data = NULL );

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
       virtual bool DoCreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::uint8_t* data ) = 0;

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

	};
}

#endif