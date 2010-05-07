#include "Sp_Exception.hpp"
#include <boost/lexical_cast.hpp>

std::string& Spiral::BaseException::GetGeneralError() const throw()
{
	m_final = GetComponentName() + m_errorStr + "\n";

	boost::shared_ptr<std::string const> fileInfo(boost::get_error_info<Ex_SrcFileInfo>( *this ));
	boost::shared_ptr<std::string const> functionInfo(boost::get_error_info<Ex_FunctionInfo>( *this ));
	boost::shared_ptr<int const> lineInfo(boost::get_error_info<Ex_SrcLineInfo>( *this ));

	if( fileInfo )
	{
		m_final += "Src File: " + *fileInfo + "\n";
	}

	if( functionInfo )
	{
		m_final += "Function: " + *functionInfo + "\n";
	}

	if( lineInfo )
	{
		m_final += "Line: " + boost::lexical_cast< std::string >( *lineInfo ) + "\n";
	}

	return m_final;
}