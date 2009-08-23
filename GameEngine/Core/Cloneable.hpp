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

		boost::shared_ptr< Cloneable > Clone()const;
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