/*
*/
#ifndef CLONEABLE_HPP
#define CLONEABLE_HPP

#include <boost/shared_ptr.hpp>

namespace Spiral
{
	class Cloneable
	{
	public:
		virtual ~Cloneable();

		const boost::shared_ptr< Cloneable > Clone()const;

		template< class To >
		const boost::shared_ptr< To > Clone_Cast()const
		{
			return boost::shared_ptr< To >( static_cast<To*>( DoClone() ) );
		}

	private:
		/*!
			@function   DoClone
			@brief  	 returns a copy
			@return     Cloneable*
		*/
		virtual Cloneable* DoClone()const = 0;
	protected:
		Cloneable();
	};
}

#endif