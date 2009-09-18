#include <sstream>
#include <string>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

#include "File.hpp"

using namespace boost;
using namespace Spiral;

int32_t IFile::Read( boost::int8_t* buffer, boost::int32_t count )
{
	return DoRead( buffer, count );
}

uint32_t IFile::Size()const
{
	return DoSize();
}

void IFile::Seek( seek_e position, boost::int32_t bytes )
{
	DoSeek( position, bytes );
}

void IFile::Close()
{
	DoClose();
}

bool IFile::Eof()const
{
	return DoEof();
}

int32_t OFile::Write( const boost::int8_t* buffer, boost::int32_t count )
{
	return DoWrite( buffer, count );
}

void OFile::Close()
{
	DoClose();
}

void OFile::WriteOutStream( const boost::shared_ptr< std::ostream >& stream )
{
	std::ostringstream* strStream = static_cast< std::ostringstream* >( stream.get() );
	std::string buff = strStream->str();
	DoWrite( reinterpret_cast< const boost::int8_t* >( buff.c_str() ), buff.length() );
}

boost::shared_ptr< std::ostream > OFile::GetStream() const
{
	std::ostringstream* strStream = new std::ostringstream;
	shared_ptr< std::ostream > newStream( strStream );
	return newStream;
}

boost::shared_ptr< std::istream > IFile::GetStream()
{
	shared_ptr< std::istream > stream;
	if( !DoEof() )
	{
		uint32_t size = DoSize();
		scoped_array< char > array( new char[ size + 1 ] );
		
		array[ size ] = NULL;
		DoRead( reinterpret_cast<int8_t*>( array.get() ), size );

		stream.reset( new std::istringstream( std::string( reinterpret_cast<const char*>(array.get()), size ) ) );
	}

	return stream;
}