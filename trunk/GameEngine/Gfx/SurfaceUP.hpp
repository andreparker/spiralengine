#ifndef SURFACE_UP_HPP
#define SURFACE_UP_HPP

#include "Surface.hpp"
#include <boost/make_shared.hpp>

namespace Spiral
{

	/*!
		@class SurfaceUP
		@brief used to provide surface functionality without allocation
	*/
	class SurfaceUP : public Surface
	{
	public:
		virtual ~SurfaceUP()
		{
		}
		SurfaceUP( boost::int32_t width, boost::int32_t height, boost::int32_t elementSize, boost::int8_t* dataPtr ):
		  Surface( width, height, elementSize, 0 ), m_userPtr( dataPtr ){}
  
		void SetDataPtr( boost::int8_t* ptr )
		{
			m_userPtr = ptr;
		}

		virtual const boost::int8_t* GetConstData()const
		{
			return m_userPtr;
		}
	private:
		boost::int8_t* m_userPtr;
	};

	inline boost::shared_ptr<SurfaceUP> make_surfaceUP( boost::int32_t width, boost::int32_t height, 
											boost::int32_t elementSize, boost::int8_t* dataPtr )
	{
		return boost::make_shared<SurfaceUP>( width, height, elementSize, dataPtr );
	}
}

#endif