#ifndef FONT_FACTORY_HPP
#define FONT_FACTORY_HPP

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include "../Core/File.hpp"

namespace Spiral
{
	class Font;
	class FontFactory : boost::noncopyable
	{
	public:
		virtual ~FontFactory();

		/*!
		   @function  Initialize
		   @brief     initializes the factory
		   @return    bool
		*/
		bool Initialize();

		
		/*!
		   @function  CreateFont
		   @brief     creates a font object from file
		   @return    boost::shared_ptr< Font > - font
		   @param     const boost::shared_ptr< IFile > & fontFile - font file handle
		   @param     boost::int32_t charWidth - width of font in pixels
		   @param     boost::int32_t charHeight - ...
		*/
		boost::shared_ptr< Font > CreateFont( const boost::shared_ptr< IFile >& fontFile, boost::int32_t charWidth, boost::int32_t charHeight );
	protected:
		FontFactory();

	private:
		virtual bool DoInitialize() = 0;
		virtual boost::shared_ptr< Font > DoCreateFont( const boost::shared_ptr< IFile >& fontFile, boost::int32_t charWidth, boost::int32_t charHeight  ) = 0;
	};
}

#endif