/*!
*/
#ifndef OGL_HW_VERTEX_BUFFER_RESOURCE_HPP
#define OGL_HW_VERTEX_BUFFER_RESOURCE_HPP

#include <boost/thread/mutex.hpp>
#include "../../Resource/Resource.hpp"

namespace Spiral
{
	class OglHWVertexBufferResource : public Resource
	{
	public:
		OglHWVertexBufferResource( boost::int32_t size );
		~OglHWVertexBufferResource();

		void Bind();
		void UnBind();
		
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
		bool m_isValid;			///< valid
		boost::int32_t m_bufferId;
		boost::int32_t m_size;
		boost::mutex m_mutex;
	};
}

#endif