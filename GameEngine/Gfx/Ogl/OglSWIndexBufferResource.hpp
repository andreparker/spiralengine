/*!
*/
#ifndef OGL_SW_INDEX_BUFFER_RESOURCE_HPP
#define OGL_SW_INDEX_BUFFER_RESOURCE_HPP

#include <boost/thread/mutex.hpp>
#include "../../Resource/Resource.hpp"

namespace Spiral
{
	class OglSWIndexBufferResource : public Resource
	{
	public:
		OglSWIndexBufferResource( boost::int32_t size );
		~OglSWIndexBufferResource();

		boost::int8_t* GetData()
		{
			return m_data;
		}

		const boost::int8_t* GetData()const
		{
			return m_data;
		}

	private:


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
		@param 	  const Rect< boost::int32_t > & rect - rectangle bounds to lock
		@param 	  ResLockRtInfo & info - struct containing lock information
		@param 	  bool bDiscard - whether or not to discard current data
		*/
		virtual bool DoLock( const Rect< boost::int32_t >& rect, ResLockRtInfo_t& info, bool bDiscard );

		/*!
		@function   DoUnlock
		@brief  	   unlocks the data
		@return    	void
		*/
		virtual void DoUnlock();

		virtual boost::int32_t DoSize()const;

		virtual void DoSize( Rect< boost::int32_t >& rect )const;
	private:
		bool m_isValid;			///< valid
		boost::int8_t* m_data;
		boost::int32_t m_size;
		boost::mutex m_mutex;
	};
}

#endif