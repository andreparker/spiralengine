#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <loki/ScopeGuard.h>

#include "../Core/File.hpp"
#include "../Core/FileManager.hpp"
#include "../Core/GeneralException.hpp"
#include "../Common/String/Utf8.hpp"
#include "StringLocalizer.hpp"


using namespace Spiral;
using namespace Spiral::locale;
using namespace boost;
using namespace boost::property_tree;
using namespace Spiral::Common;

StringLocalizer::StringLocalizer():
m_table(),m_stringPath()
{
}

StringLocalizer::~StringLocalizer()
{
	ClearStringTable();
}


bool StringLocalizer::LoadStringFile( const cString& fileName )
{
	bool isLoaded = false;
	shared_ptr< IFile > file;
	if( FileManager::instance().getFile( MakeFullPath(fileName), file ) )
	{
		ptree xml_data;
		Loki::ScopeGuard guard = Loki::MakeObjGuard( *file, &IFile::Close );
		boost::shared_ptr< std::istream > pStream = file->GetStream();
		try
		{
			xml_parser::read_xml( *pStream, xml_data );
			ParseXmlTree( xml_data );
			isLoaded = true;
		}catch( std::exception& e )
		{
			std::string nl = "\n";
			THROW_GENERAL_EXCEPTION( "Error loading - " + fileName + nl + e.what() );
		}
	}

	return isLoaded;
}

void StringLocalizer::ClearStringTable()
{
	m_table.clear();
}

const wString& StringLocalizer::GetString( const cString& key ) const
{
	string_table_const_itr itr = m_table.find( key );
	if( itr == m_table.end() )
	{
		THROW_GENERAL_EXCEPTION( "Error could not find string ID: " + key + "\n" );
	}

	return itr->second;
}

cString StringLocalizer::MakeFullPath( const cString& fileName ) const
{
	if( m_stringPath.length() )
	{
		const cString::value_type* str = ( m_stringPath.c_str() + ( m_stringPath.length() - 1 ) );
		if(  *str == '\\' || *str == '/' )
		{
			return cString( m_stringPath + fileName );
		}else
		{
			return cString( m_stringPath + '/' + fileName );
		}
	}

	return fileName;
}

void StringLocalizer::ParseXmlTree( const boost::property_tree::ptree& tree )
{
	BOOST_FOREACH( const ptree::value_type& node, tree.get_child("strings") )
	{
		// we should have a id node
		if( node.first == "id" )
		{
			const ptree& xmlAttr = node.second.get_child( "<xmlattr>" );
			const ptree& value   = xmlAttr.get_child( "value" );
			const cString& key   = value.data();

			const ptree& string_value = node.second.get_child( "string" );
			wString str = String::ConvertUtf8_to_Wchar( string_value.data() );
			m_table.insert( std::pair< cString, wString >( key, str ) );
		}else
		{
			THROW_GENERAL_EXCEPTION( "Error invalid string file, no \"id\" found!" );
		}
	
	}
}