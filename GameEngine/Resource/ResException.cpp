#include <boost/lexical_cast.hpp>
#include "ResException.hpp"


using namespace Spiral;
using namespace std;

using namespace boost;

ResException::ResException( const string& errorStr ) throw():
BaseException( "Resource", errorStr )
{
}

const char* ResException::what() const throw()
{
    std::string& final = GetGeneralError();
    AddResInfo( final );

    return final.c_str();
}

void ResException::AddResInfo( std::string& errorStr ) const throw()
{
    boost::shared_ptr< boost::int32_t const > sizePtr = get_error_info< res_size_info >( *this );
    if( sizePtr )
    {
        errorStr += "[ Resource Size ] " + lexical_cast< string >( *sizePtr ) + "\n";
    }

    boost::shared_ptr< string const > typePtr = get_error_info< res_type_info >( *this );
    if( typePtr )
    {
        errorStr += "[ Resource Type ] " + *typePtr + "\n";
    }
}