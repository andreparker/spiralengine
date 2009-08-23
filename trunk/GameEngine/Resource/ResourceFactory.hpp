/*!
*/
#ifndef RESOURCE_FACTORY_HPP
#define RESOURCE_FACTORY_HPP

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include "../Core/TypeUtils.hpp"
#include "../Core/Singleton.ipp"
#include "Resource.hpp"

namespace Spiral
{

namespace detail
{
    template< class ResPolicyClass >
    class ResourceFactory : public Singleton< ResourceFactory< ResPolicyClass > >
    {
        DECL_SINGLETON( ResourceFactory< ResPolicyClass > )
    public:

        template< class ResourceType >
        boost::shared_ptr< ResourceType > Create( boost::int32_t size, EmptyType< ResourceType > );
        
        /*!
        	@function   Create
        	@brief  	creates a managed resource using ResPolicyClass
        	@return    	boost::shared_ptr< ManagedResource >
        	@param 		boost::int32_t size - size of the resource in bytes
        	@param 		EmptyType< ManagedResource >
        */
        template<>
        boost::shared_ptr< ManagedResource > Create( boost::int32_t size, EmptyType< ManagedResource > );

        /*!
        	@function   Create
        	@brief  	creates a resource using ResPolicyClass
        	@return    	boost::shared_ptr< Resource >
        	@param 		boost::int32_t size - size of the resource in bytes
        	@param 		EmptyType< Resource >
        */
        template<>
        boost::shared_ptr< Resource > Create( boost::int32_t size, EmptyType< Resource > );
    };

    template<ResPolicyClass>
    boost::shared_ptr< ManagedResource > ResourceFactory<ResPolicyClass>::Create( boost::int32_t size, EmptyType< ManagedResource > )
    {
        boost::shared_ptr< ManagedResource > res = ResPolicyClass::Create( size, EmptyType< ManagedResource > );

        return res;
    }

    template<ResPolicyClass>
    boost::shared_ptr< Resource > ResourceFactory<ResPolicyClass>::Create( boost::int32_t size, EmptyType< Resource > )
    {
        boost::shared_ptr< Resource > res = ResPolicyClass::Create( size, EmptyType< Resource > );

        return res;
    }

}// detail

}

#endif