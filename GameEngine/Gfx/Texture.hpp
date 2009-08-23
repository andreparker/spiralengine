/*!
*/
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include "../Core/Cloneable.hpp"
#include "../Resource/ResourceFwd.hpp"

namespace Spiral
{

    class Texture : private boost::noncopyable, public Cloneable
    {
    public:
		virtual ~Texture();
        /*!
           @function   GetWidth
           @brief  	  returns the width
           @return     boost::int32_t
        */
        boost::int32_t GetWidth()const
        {
            return m_width;
        }

        /*!
           @function   GetHeight
           @brief  	  returns the height
           @return     boost::int32_t
        */
        boost::int32_t GetHeight()const
        {
            return m_height;
        }

        /*!
           @function   GetBitDepth
           @brief  	  bit depths 16, 32
           @return     boost::int32_t
        */
        boost::int32_t GetBitDepth()const
        {
            return m_bitDepth;
        }

        /*!
           @function   GetResource
           @brief  	  gets the textures resource
           @return     boost::shared_ptr< Resource >
        */
        const boost::shared_ptr< Resource > GetResource()const
        {
            return m_resource;
        }

		boost::shared_ptr< Resource > GetResource()
		{
			return m_resource;
		}


    protected:
        Texture();

	private:
        boost::int32_t m_width;
        boost::int32_t m_height;
        boost::int32_t m_bitDepth;

        boost::shared_ptr< Resource > m_resource;
	
		/*!
			@function   DoClone
			@brief  	  returns a copy
			@return     Cloneable*
		*/
		virtual Cloneable* DoClone()const;
    };
}

#endif