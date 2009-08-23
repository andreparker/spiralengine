#include "Binaryfile.hpp"

using namespace Spiral;
using namespace std;
using namespace boost;

IBinaryFile::IBinaryFile( ifstream* ifs ):
m_fileSize(0),m_istream( ifs )
{
	if( m_istream->is_open() )
	{
		m_istream->seekg( 0, ifstream::end );
		m_fileSize = static_cast< uint32_t >( m_istream->tellg() );
		m_istream->seekg( 0 );
	}
}

int32_t IBinaryFile::DoRead( int8_t* buffer, int32_t count)
{
	m_istream->read( reinterpret_cast<char*>(buffer) , count  );
	return m_istream->bad() ? 0 : count; 
}

uint32_t IBinaryFile::DoSize() const
{
	return m_fileSize;
}

void IBinaryFile::DoSeek( seek_e /*&position*/, int32_t /*bytes*/ )
{
}

void IBinaryFile::DoClose()
{
	m_istream->close();
}

bool IBinaryFile::DoEof() const
{
	return m_istream->eof();
}

OBinaryFile::OBinaryFile( ofstream* ofs ):
m_ostream( ofs )
{
}

int32_t OBinaryFile::DoWrite(const int8_t* buffer, int32_t count)
{
	m_ostream->write( reinterpret_cast< const char* >(buffer) , count );
	return m_ostream->bad() ? 0 : count;
}

void OBinaryFile::DoClose()
{
	m_ostream->close();
}
 