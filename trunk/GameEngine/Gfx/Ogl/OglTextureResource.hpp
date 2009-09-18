#ifndef OGL_TEXTURE_RESOURCE_HPP
#define OGL_TEXTURE_RESOURCE_HPP

#include <boost/thread/mutex.hpp>
#include <boost/scoped_array.hpp>

#include "../../Resource/Resource.hpp"
#include "../GfxImplFwd.hpp"
#include "../../Core/Rect.hpp"

namespace Spiral
{


	class OglTextureResource : public Resource
	{
	public:
		~OglTextureResource();
		OglTextureResource( const TextureInfo_t& info, const boost::int8_t* data );


		boost::uint32_t GetTextureID()const
		{
			return m_oglTextureId;
		}
	private:
		OglTextureResource();
		/*!
        	@function   DoIsValid
        	@brief  	   checks if the resource is valid
        	@return    	bool
        */
        virtual bool DoIsValid()const;

        /*!
           @function   DoLock
           @brief  	   locks the resource and returns a pointer to it
           @return    	bool - true on success
           @param      boost::int32_t start - start position to lock
           @param 		boost::int32_t size - how much of the resource to lock
           @param      ResLockInfo& info - struct containing returned lock information
           @param 		bool bDiscard - whether or not to discard current data
        */
        virtual bool DoLock( boost::int32_t start, boost::int32_t size, ResLockInfo_t& info, bool bDiscard );

        /*!
           @function   DoLock
           @brief  	  locks a rectangle portion of the resource i.e Texures/images
           @return     bool
           @param 	  ResLockRtInfo & info - struct containing lock information
           @param 	  bool bDiscard - whether or not to discard current data
        */
        virtual bool DoLock( ResLockRtInfo_t& info, bool bDiscard );

        /*!
        	  @function   DoUnlock
        	  @brief  	   unlocks the data
        	  @return    	void
        */
        virtual void DoUnlock();

        virtual boost::int32_t DoSize()const;

        virtual void DoSize( Rect< boost::int32_t >& rect )const;
	private:
		boost::uint32_t m_oglTextureId;
		boost::int32_t m_colorChannels;
		bool m_isValid;
		Rect< boost::int32_t > m_rect;
		boost::scoped_array<boost::int8_t> m_data;
		boost::mutex m_mutex;

	};
}

#endif