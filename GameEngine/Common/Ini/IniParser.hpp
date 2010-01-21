#ifndef INI_PARSER_HPP
#define INI_PARSER_HPP

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/assert.hpp>

#include "../String/String.hpp"
#include "../../Core/Sp_DataTypes.hpp"

namespace Spiral { namespace Common { namespace Ini {

	class IniParser
	{
	public:

		typedef boost::property_tree::ptree Tree;
		typedef Tree::iterator              Tree_Iter;
		typedef Tree::const_iterator        Tree_Const_Iter;

		IniParser();
		IniParser( const cString& fileName );

		template< class ValType >
		const ValType GetValue( const cString& key )const
		{
			BOOST_ASSERT( m_tree.find( key ) != m_tree.end() );
			return m_tree.get< ValType >( key );
		}

		void Parse( const cString& fileName );

	private:
		Tree m_tree;
	};

	template<>
	const cString IniParser::GetValue( const cString& key )const
	{
		Tree_Const_Iter itr = m_tree.find( key );
		BOOST_ASSERT( itr != m_tree.end() );
		return itr->second.data();
	}

	template<>
	const wString IniParser::GetValue( const cString& key )const
	{
		Tree_Const_Iter itr = m_tree.find( key );
		BOOST_ASSERT( itr != m_tree.end() );
		return String::ConvertCString_To_WString( itr->second.data() );
	}

} // Ini
} // Common

}
#endif