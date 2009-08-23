/*!
*/
#ifndef WIN_OGL_EXCEPTION_HPP
#define WIN_OGL_EXCEPTION_HPP

#include "../../Core/Sp_Exception.hpp"

namespace Spiral
{
    class WinOglException : public BaseException
    {
    public:
        WinOglException( const std::string& errorStr );

        virtual const char* what()const throw();
    };

}


#endif