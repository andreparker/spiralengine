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