#include <boost/shared_ptr.hpp>
#include <loki/ScopeGuard.h>

#include "../../Core/GeneralException.hpp"
#include "../../Core/FileManager.hpp"
#include "../../Core/File.hpp"
#include "IniParser.hpp"


using namespace boost;
using namespace boost::property_tree;

namespace Spiral { namespace Common { namespace Ini {


	void IniParser::Parse( const cString& fileName )
	{
		try
		{
			shared_ptr< IFile > iniFile;
			if( FileManager::instance().getFile( fileName, iniFile ) )
			{
				Loki::ScopeGuard guard = Loki::MakeObjGuard( *iniFile, &IFile::Close );

				shared_ptr< std::istream > pStream = iniFile->GetStream();
				ini_parser::read_ini( *pStream, m_tree );

			}
		}
		catch( std::exception& e )
		{
			THROW_GENERAL_EXCEPTION( "Error loading ini: " + fileName + "\n" + e.what() );
		}
	}

	IniParser::IniParser():
	m_tree()
	{
	}

	IniParser::IniParser( const cString& fileName )
	{
		Parse( fileName );
	}
}
}
}