#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

namespace Spiral
{
	class Texture;
	class GfxDriver;

	class Surface : boost::noncopyable
	{
	public:
		/*!
		   @function  Surface
		   @brief     constructs a surface
		   @return    
		   @param     boost::int32_t width - width
		   @param     boost::int32_t height - ...
		   @param     boost::int32_t elementSize - size in bytes of each element
		*/
		Surface( boost::int32_t width, boost::int32_t height, boost::int32_t elementSize );
		~Surface();

		/*!
		   @function  CreateTextureFromData
		   @brief     creates a texture from suface data
		   @return    boost::shared_ptr< Texture >
		   @param     const boost::shared_ptr< GfxDriver > & driver
		*/
		boost::shared_ptr< Texture > CreateTextureFromData( const boost::shared_ptr< GfxDriver >& driver );

		/*!
		   @function  Clear
		   @brief     clears the surface to value
		   @return    void
		   @param     boost::int32_t value
		*/
		void Clear( boost::int32_t value = 0 );

		/*!
		   @function  CreateRgb24bitSurface
		   @brief     creates a 24 bit surface
		   @return    boost::shared_ptr< Surface >
		   @param     boost::int32_t width
		   @param     boost::int32_t height
		*/
		static boost::shared_ptr< Surface > CreateRgb24bitSurface( boost::int32_t width, boost::int32_t height );

		/*!
		   @function  CreateRgba32bitSurface
		   @brief     creates a 32 bit surface
		   @return    boost::shared_ptr< Surface >
		   @param     boost::int32_t width
		   @param     boost::int32_t height
		*/
		static boost::shared_ptr< Surface > CreateRgba32bitSurface( boost::int32_t width, boost::int32_t height );

		boost::int32_t GetWidth()const
		{
			return m_width;
		}

		boost::int32_t GetHeight()const
		{
			return m_height;
		}

		const boost::int8_t* GetConstData()const
		{
			return m_data.get();
		}

		boost::int32_t GetBytesPerLine()const
		{
			return m_elementSize * m_width;
		}

		boost::int32_t GetElementSize()const
		{
			return m_elementSize;
		}

		boost::int8_t* GetData()
		{
			return const_cast< boost::int8_t* >( GetConstData() );
		}
	protected:
		Surface();

	private:
		boost::int32_t m_width,m_height,m_elementSize;
		boost::scoped_array< boost::int8_t > m_data;
	};

	inline boost::shared_ptr< Surface > make_surface( boost::int32_t width, boost::int32_t height, boost::int32_t elementSize )
	{
		return boost::shared_ptr< Surface >( new Surface( width, height, elementSize ) );
	}
}
#endif