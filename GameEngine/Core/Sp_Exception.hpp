/*!
*/
#ifndef SP_EXCEPTION_HPP
#define SP_EXCEPTION_HPP

#include <boost/exception.hpp>
#include <exception>
#include <string>

namespace Spiral
{
	typedef boost::error_info<struct src_file_info_tag, std::string> Ex_SrcFileInfo;
	typedef boost::error_info<struct function_info_tag, std::string> Ex_FunctionInfo;
	typedef boost::error_info<struct src_line_info_tag, int >        Ex_SrcLineInfo;

	class BaseException : public std::exception, public boost::exception
	{
	protected:
		 BaseException( const std::string& component, const std::string& errorStr ) throw():
			 m_componentName( component ),m_errorStr( errorStr ){}

		 std::string GetComponentName()const throw()
		 {
			 return std::string( "[ " + m_componentName + " ] " );
		 }

		 std::string& GetGeneralError()const throw();

		 std::string& GetFinal()const
		 {
			 return m_final;
		 }

	private:
		std::string m_componentName;
		std::string m_errorStr;
		mutable std::string m_final;

	};
}

#define THROW_BASIC_EXCEPTION( ExClass ) throw ExClass << Spiral::Ex_SrcFileInfo( __FILE__ ) \
	<< Spiral::Ex_FunctionInfo( __FUNCTION__ ) << Spiral::Ex_SrcLineInfo( __LINE__ )

#endif