/*!
*/
#ifndef RES_EXCEPTION_HPP
#define RES_EXCEPTION_HPP

#include <boost/cstdint.hpp>

#include "../Core/Sp_Exception.hpp"


namespace Spiral
{

    typedef boost::error_info<struct res_size, boost::int32_t> res_size_info;
    typedef boost::error_info<struct res_type, std::string> res_type_info;

    class ResException : public BaseException
    {
    public:
        ResException( const std::string& errorStr ) throw();

        virtual const char* what()const throw();
    protected:

        /*!
        	   @function   AddResInfo
        	   @brief  	   adds extra information to the error string
        	   @return    	void
        	   @param 		std::string & errorStr
        */
        void AddResInfo( std::string& errorStr ) const throw();
    };
}

#endif