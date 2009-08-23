/*!
*/
#ifndef DEVICE_DRIVER_HPP
#define DEVICE_DRIVER_HPP

#include <string>
#include <boost/any.hpp>
#include <boost/utility.hpp>

namespace Spiral
{
	class DeviceDriver : public boost::noncopyable
	{
	public:

		virtual ~DeviceDriver(){}

		/*!
		   @function    GetName
		   @brief
		   @return      const std::string&
		*/
		const std::string& GetName()const
		{
			return m_name;
		}

		/*!
		   @function    GetInfo
           @brief
		   @return      const std::string&
		*/
		const std::string& GetInfo()const
		{
			return m_info;
		}

		/*!
		   @function    Initialize
           @brief       initializes data
		   @return      bool
		   @param       const boost::any & data
		*/
		bool Initialize( const boost::any& data = boost::any() );

		/*!
		   @function    UnInitialize
           @brief
		   @return      bool
		*/
		bool UnInitialize();

		/*!
		   @function  SetName
		   @brief     sets the driver name
		   @return    void
		   @param     const std::string & name
		*/
		void SetName( const std::string& name )
		{
			m_name = name;
		}

		/*!
		   @function  SetInfo
		   @brief     sets the drivers information
		   @return    void
		   @param     const std::string & info
		*/
		void SetInfo( const std::string& info )
		{
			m_info = info;
		}
	
	protected:
		DeviceDriver() : m_name( "Invalid" ),m_info( "NULL" ){}
	
	private:
		std::string m_name;
		std::string m_info;
	
		/*!
			@function    DoInitialize
			@brief       initializes data
			@return      bool
			@param       const boost::any & data
		*/
		virtual bool DoInitialize( const boost::any& data ) = 0;

		/*!
			@function    UnInitialize
			@brief
			@return      bool
		*/
		virtual bool DoUnInitialize() = 0;

	};
}

#endif