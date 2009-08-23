/*!
*/
#ifndef CVAR_HPP
#define CVAR_HPP

#include <boost/cstdint.hpp>
#include <boost/any.hpp>
#include <boost/tokenizer.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <string>
#include <map>

#include "FileFwd.hpp"
#include "TypeUtils.hpp"
#include "Singleton.ipp"

namespace Spiral
{
	typedef boost::tokenizer< boost::char_separator< char > > char_tokenizer;
	typedef std::map< const std::string, boost::function< void( char_tokenizer::iterator, char_tokenizer::iterator ) > > command_map;
	typedef std::map< const std::string, boost::any > variable_map;

	class CVar : public Singleton< CVar >
	{
		DECL_SINGLETON( CVar )
	public:

		/*!
		   @function    Initialize
		   @brief       initializes variables and function callbacks
		   @return      void
		*/
		void Initialize();

		/*!
		   @function    ProcessString
		   @brief       processes a command from a string
		   @return      void
		   @param       const std::string & str
		*/
		void ProcessString( const std::string& str );

		/*!
		   @function    ProcessFile
		   @brief       process a file with vars in it
		   @return      bool
		   @param       boost::shared_ptr< IFile >& file
		*/
		bool ProcessFile( boost::shared_ptr< IFile >& file );

		/*!

			@brief grabs the value of a variable

			@return the value
		*/
		template< class T >
		T& GetVarValue
		( 
			const std::string& varName, ///< variable name
			EmptyType< T > ///< dummy to allow template partial specialization through overload
		)const
		{
			return boost::any_cast< T >( m_vars[ varName.c_str() ] );
		}

		/*!
			@brief sets values in a variable
		*/
		template< class T >
		void SetVarValue
		(
			const std::string& varName, ///< variable
			const T& value ///< a value to set
		)
		{
			m_vars[ varName.c_str() ] = value;
		}

		/*!
		   @function  RegisterCallbacks
		   @brief     
		   @return    void
		   @param     const std::string & commandStr
		   @param     boost::function< void (char_tokenizer::iterator,char_tokenizer::iterator) > func
		*/
		void RegisterCallbacks( const std::string& commandStr, boost::function< void( char_tokenizer::iterator, char_tokenizer::iterator ) > func );

	private:
		
		variable_map m_vars;
		command_map m_commands;

		/*!
		   @function  ParseBufferText
		   @brief     parses a text buffer
		   @return    void
		   @param     boost::shared_array< char > & buffer
		   @param     const boost::int32_t bufferSize
		*/
		void ParseBufferText( boost::shared_array< char > &buffer, const boost::int32_t bufferSize );

		void ProcessTokens
		( 
			char_tokenizer::iterator first, ///< iterators to start of tokens 
			char_tokenizer::iterator last   ///< .......
		);

		/*!
		   @function    RegisterVars
		   @brief       register the variables
		   @return      void
		*/
		void RegisterVars();

		/*!
		   @function    RegisterCommands
		   @brief       register commands
		   @return      void
		*/
		void RegisterCommands();

		/*!
			@brief commands
		*/
		void Version( char_tokenizer::iterator, char_tokenizer::iterator );
		void Exec( char_tokenizer::iterator, char_tokenizer::iterator );

	};

}

#endif