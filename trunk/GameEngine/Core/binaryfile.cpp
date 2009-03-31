#include "binaryfile.hpp"

using namespace gameEngine;

IBinaryFile::IBinaryFile(FILE *fileHandle):
m_fileSize(0),m_fileHandle( fileHandle )
{
	if( fileHandle )
	{
		fseek( fileHandle, 0, SEEK_END );
		m_fileSize = static_cast< uint32 >( ftell( fileHandle ) );
		fseek( fileHandle, 0, SEEK_SET );
	}
}

uint32 IBinaryFile::read(void *buffer, uint32 count)
{
	return static_cast< uint32 >( fread( buffer, sizeof(char), count, m_fileHandle ) );
}

uint32 IBinaryFile::size() const
{
	return m_fileSize;
}

void IBinaryFile::seek(const ENUMDEF2OBJECT( seek_e )& /*&position*/, uint32 /*bytes*/ )
{
}

void IBinaryFile::close()
{
	fclose( m_fileHandle );
	delete this;
}

bool IBinaryFile::eof() const
{
	return bool( feof( m_fileHandle ) > 0 );
}

OBinaryFile::OBinaryFile(FILE *fileHandle):
m_fileHandle( fileHandle )
{
}

uint32 OBinaryFile::write(const void *buffer, uint32 count)
{
	return static_cast< uint32 >( fwrite( buffer, sizeof(char), count, m_fileHandle ) );
}

void OBinaryFile::close()
{
	fclose( m_fileHandle );
	delete this;
}