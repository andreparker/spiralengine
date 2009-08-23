/*!
*/
#ifndef SP_EXCEPTION_HPP
#define SP_EXCEPTION_HPP

#include <boost/exception.hpp>
#include <exception>
#include <string>

namespace Spiral
{
	class BaseException : public std::exception, public boost::exception
	{
	protected:
		 BaseException( const std::string& component, const std::string& errorStr ) throw():
			 m_componentName( component ),m_errorStr( errorStr ){}

		 std::string GetComponentName()const throw()
		 {
			 return std::string( "[ " + m_componentName + " ] " );
		 }

		 std::string& GetGeneralError()const throw()
		 {
			 m_final = GetComponentName() + m_errorStr + "\n";
			 return m_final;
		 }

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

#endif