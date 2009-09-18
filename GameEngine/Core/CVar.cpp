/*!
*/

#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <algorithm>

#include "CVar.hpp"
#include "TypeUtils.hpp"
#include "FileManager.hpp"


using namespace boost::assign;
using namespace Spiral;
using namespace boost;
using namespace std;

// delimiters
char_separator<char> com_delims(",|; ");
const int8_t COMMAND_TOKEN = '\\';


typedef boost::function< void( char_tokenizer::iterator, char_tokenizer::iterator ) > tokenFunc;

inline string TrimToken( char_tokenizer::iterator token )
{
	string tokenStr = *token;
	trim( tokenStr );
	return tokenStr;
}


void CVar::Initialize()
{
	RegisterVars();
	RegisterCommands();
}

void CVar::ProcessTokens( char_tokenizer::iterator first, char_tokenizer::iterator last )
{
	/*
		check to see if we are messing with cvars
	*/
	char ctoken = *((*first).c_str());
	if( ctoken == COMMAND_TOKEN )
	{
		// process commands
		string command = *first;
		trim_left_if( command, equal_to_val<char,'\\'>() );
		if( m_commands.find( command ) != m_commands.end() )
		{
			// someone tell me why I cannot do (first + 1) ????
			m_commands[ command ]( ++first /*+ 1*/, last );
		}
		else
		{
			// log out invalid command string
		}
	}
}

void CVar::RegisterCallbacks( const string& commandStr, boost::function< void( char_tokenizer::iterator, char_tokenizer::iterator ) > func )
{
	if( m_commands.find( commandStr ) == m_commands.end() )
	{
		m_commands.insert( pair<string,tokenFunc>( commandStr, func ) );
	}
}

void CVar::RegisterVars()
{
	string version = "1.0";
	int32_t value = 0;

	m_vars = map_list_of
	( "version", any(version) )
	( "vid_width", any(value) )
	( "vid_height", any(value) )
	( "vid_fullscreen", any(value) );
}

void CVar::RegisterCommands()
{
	m_commands = map_list_of
	( "version", bind( &CVar::Version, this, _1, _2 ) )
	( "exec"   , bind( &CVar::Exec, this, _1, _2 ) )
	( "vid_width", bind( &CVar::ReadInt_vid_width, this, _1, _2 ) )
	( "vid_height", bind( &CVar::ReadInt_vid_height, this, _1, _2 ) )
	( "vid_fullscreen", bind( &CVar::ReadInt_vid_fullscreen, this, _1, _2 ) );
}

void CVar::ProcessString(const std::string &str)
{
	/*
		parse and tokenize command string
		get token count to pass to program_options parser
	*/
	char_tokenizer tokens( str, com_delims );
	ProcessTokens( tokens.begin(), tokens.end() );
	
}

bool CVar::ProcessFile( shared_ptr< IFile >& file )
{
	bool processed = false;

	if( file && file->Size() > 0 )
	{
		const int32_t fileSize = file->Size(); 
		shared_array< char > buffer( new char[ fileSize + 1 ] );
		int32_t sizeRead = file->Read( reinterpret_cast<int8_t*>(buffer.get()), fileSize );
		if( sizeRead == fileSize )
		{
			buffer[ fileSize ] = NULL; // null terminate the string
			ParseBufferText(buffer, fileSize);
			processed = true;
		}

	}

	return processed;
}

/*
	Commands
*/
void CVar::Version( char_tokenizer::iterator, char_tokenizer::iterator )
{
	/*
		print version number
	*/
}

void CVar::Exec( char_tokenizer::iterator first, char_tokenizer::iterator last )
{
	shared_ptr< IFile > file;
	if( FileManager::instance().getFile( *first, file ) )
	{
		File_Auto_Close<IFile> auto_close( file );

		if( !ProcessFile( file ) )
		{
			// could not open this file, print error
		}
	}
}

void CVar::ParseBufferText( shared_array< char > &buffer, const int32_t bufferSize )
{
	char   *itrFirst = buffer.get(),
		   *itrTmp = itrFirst,
		   *itrLast = itrFirst + bufferSize;

	std::string tmpStr;
	while( itrTmp != itrLast )
	{
		// ( *itrCurrent == '\n' )
		// find the end of the string
		itrTmp = find_if( itrFirst, itrLast, equal_to_val<char,'\n'>() );
		if( itrTmp != itrLast )
		{
			*itrTmp = NULL;
			tmpStr = itrFirst; // null the found newLine and let string copy

			ProcessString( tmpStr );
			itrFirst = itrTmp + 1;		// move to new starting point
		}
	}
}

void CVar::ReadInt_vid_width( char_tokenizer::iterator lhs, char_tokenizer::iterator rhs)
{
	string number = TrimToken( lhs );
	m_vars["vid_width"] = lexical_cast<int32_t>( number );
}

void CVar::ReadInt_vid_height( char_tokenizer::iterator lhs, char_tokenizer::iterator rhs)
{
	string number = TrimToken( lhs );
	m_vars["vid_height"] = lexical_cast<int32_t>( number );
}

void CVar::ReadInt_vid_fullscreen( char_tokenizer::iterator lhs, char_tokenizer::iterator rhs)
{
	string number = TrimToken( lhs );
	m_vars["vid_fullscreen"] = lexical_cast<int32_t>( number );
}

CVar::CVar():
	m_vars(),
	m_commands()
{

}

