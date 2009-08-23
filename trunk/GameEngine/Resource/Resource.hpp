/*!
*/
#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <boost/any.hpp>
#include <boost/cstdint.hpp>
#include <boost/utility.hpp>

#include "ResLockImplFwd.hpp"
#include "../Core/RectFwd.hpp"

namespace Spiral
{

	class Resource : private boost::noncopyable
    {
	public:
		virtual ~Resource();

		/*!
			@function   IsValid
			@brief  	   checks if the resource is valid
			@return    	bool
		*/
		bool IsValid()const;

		/*!
			@function   Lock
			@brief  	locks the resource and returns a pointer to it
			@return    	bool - true on success
			@param      boost::int32_t start - start position to lock
			@param 		boost::int32_t size - how much of the resource to lock
			@param      ResLockInfo& info - struct containing returned lock information
			@param 		bool bDiscard - whether or not to discard current data
		*/
		bool Lock( boost::int32_t start, boost::int32_t size, ResLockInfo_t& info, bool bDiscard = true );

		/*!
			@function   Lock
			@brief  	  locks a rectangle portion of the resource i.e Texures/images
			@return     bool
			@param 	  const Rect< boost::int32_t > & rect - rectangle bounds to lock
			@param 	  ResLockRtInfo & info - struct containing lock information
			@param 	  bool bDiscard - whether or not to discard current data
		*/
		bool Lock( const Rect< boost::int32_t >& rect, ResLockRtInfo_t& info, bool bDiscard = true );

		/*!
			@function   Unlock
			@brief  	   unlocks the data
			@return    	void
		*/
		void Unlock();

		boost::uint32_t Size()const;

		void Size( Rect< boost::int32_t >& rect )const;

	protected:
		Resource();

    private:
		

        /*!
        	@function   DoIsValid
        	@brief  	   checks if the resource is valid
        	@return    	bool
        */
        virtual bool DoIsValid()const = 0;

        /*!
           @function   DoLock
           @brief  	   locks the resource and returns a pointer to it
           @return    	bool - true on success
           @param      boost::int32_t start - start position to lock
           @param 		boost::int32_t size - how much of the resource to lock
           @param      ResLockInfo& info - struct containing returned lock information
           @param 		bool bDiscard - whether or not to discard current data
        */
        virtual bool DoLock( boost::int32_t start, boost::int32_t size, ResLockInfo_t& info, bool bDiscard ) = 0;

        /*!
           @function   DoLock
           @brief  	  locks a rectangle portion of the resource i.e Texures/images
           @return     bool
           @param 	  const Rect< boost::int32_t > & rect - rectangle bounds to lock
           @param 	  ResLockRtInfo & info - struct containing lock information
           @param 	  bool bDiscard - whether or not to discard current data
        */
        virtual bool DoLock( const Rect< boost::int32_t >& rect, ResLockRtInfo_t& info, bool bDiscard ) = 0;

        /*!
        	  @function   DoUnlock
        	  @brief  	   unlocks the data
        	  @return    	void
        */
        virtual void DoUnlock() = 0;

        virtual boost::int32_t DoSize()const = 0;

        virtual void DoSize( Rect< boost::int32_t >& rect )const = 0;

    };

    class ManagedResource : public Resource
    {
	public:
		virtual ~ManagedResource();

		/*!
			@function   Reload
			@brief  	If resource is invalid, this function reloads data to make it valid
			@return    	bool
		*/
		bool Reload();

	protected:
		ManagedResource();

	private:
        /*!
           @function   DoReload
           @brief  	   If resource is invalid, this function reloads data to make it valid
           @return    	bool
        */
        virtual bool DoReload() = 0;
    };
}

#endif