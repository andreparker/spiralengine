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
	return m_istream->gcount();
}

uint32_t IBinaryFile::DoSize() const
{
	return m_fileSize;
}

void IBinaryFile::DoSeek( seek_e position, int32_t bytes )
{
	ios_base::seekdir direction;

	if( m_istream->eof() )
	{
		m_istream->clear();
	}
	
	switch( position )
	{
	case SK_BEGIN:
		direction = ios_base::beg;
		break;
	case SK_CUR:
		direction = ios_base::cur;
		break;
	case SK_END:
		direction = ios_base::end;
		break;
	default:
		direction = ios_base::beg;
	}

	m_istream->seekg( bytes, direction );
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
 