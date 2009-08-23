#include "zipfile.hpp"


using namespace Spiral;
using namespace boost;

IZipFile::IZipFile(HZIP handle, ZIPENTRY *entryInfo, int fileIdx):
m_zipHandle( handle ), m_entryInfo( *entryInfo ),m_fileIdx( fileIdx ),m_isEof( false ){}


/*!

@brief gets the size of the file

@return size of the file
*/
uint32_t IZipFile::DoSize() const
{
	return static_cast< uint32_t >( m_entryInfo.unc_size );
}

/*!

@brief reads num bytes from a file

@return num bytes read
*/
int32_t IZipFile::DoRead
( 
 int8_t* buffer, ///< pointer to a buffer to recieve data  
 int32_t count ///< number of bytes to read
 )
{
	if( !Eof() )
	{
		ZRESULT r = UnzipItem( m_zipHandle, m_fileIdx, buffer, static_cast< unsigned int >( count ) );
		if( r == ZR_OK )
		{
			m_isEof = true;
		}

		return count;
	}

	return 0;
}

/*!

@brief moves the read cursor
*/
void IZipFile::DoSeek
( 
 seek_e /*position*/, ///< starting position to seek
 int32_t /*bytes*/ ///< number of bytes  to seek
 )
{
	/// not yet impliminted
}

/*!

	@brief closes the file
*/
void IZipFile::DoClose()
{
	// nothing todo
}
