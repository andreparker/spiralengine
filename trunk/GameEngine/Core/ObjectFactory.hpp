/*!
*/
#ifndef OBJECT_FACTORY_HPP
#define OBJECT_FACTORY_HPP

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

#include "Singleton.ipp"

namespace Spiral
{

	class ObjectFactory : public Singleton< ObjectFactory >
	{
		DECL_SINGLETON( ObjectFactory )
	public:

		template< class T > bool Create( boost::shared_ptr< T >& newObject );
		template< class T, class Archive > bool Create( boost::shared_ptr< T >& newObject, Archive& archive );

	};
}

#endif