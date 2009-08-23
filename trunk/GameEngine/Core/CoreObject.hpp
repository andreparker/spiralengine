/*!
*/
#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include <boost/cstdint.hpp>
#include "Cloneable.hpp"
#include "Sp_DataTypes.hpp"

namespace Spiral
{
	class CoreObject : public Cloneable
	{
	public:
		virtual ~CoreObject();

		/*!
		   @function    Tick
		   @brief       Ticks the current object
		   @return      void
		   @param       real ticks < fractional seconds
		*/
		void Tick( real ticks );
		
		/*!
		   @function    GetObjectId
		   @brief       gets the objects id
		   @return      boost::int32_t
		*/
		boost::int32_t GetObjectId()const
		{
			return m_id;
		}
	protected:
		CoreObject();
	
	private:
		boost::int32_t m_id; ///< the objects id number

		/*!
			@function    DoTick
			@brief       Ticks the current object
			@return      void
			@param       real ticks < fractional seconds
		*/
		virtual void DoTick( real ticks ) = 0;


	};
}

#endif