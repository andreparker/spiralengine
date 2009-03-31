#include "zipfile.hpp"


using namespace gameEngine;


IZipFile::IZipFile(HZIP handle, ZIPENTRY *entryInfo, int fileIdx):
m_zipHandle( handle ), m_entryInfo( *entryInfo ),m_fileIdx( fileIdx ),m_isEof( false ){}


/*!

@brief gets the size of the file

@return size of the file
*/
uint32 IZipFile::size() const
{
	return static_cast< uint32 >( m_entryInfo.unc_size );
}

/*!

@brief reads num bytes from a file

@return num bytes read
*/
uint32 IZipFile::read
( 
 void* buffer, ///< pointer to a buffer to recieve data  
 uint32 count ///< number of bytes to read
 )
{
	if( !eof() )
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
void IZipFile::seek
( 
 const ENUMDEF2OBJECT( seek_e )& /*position*/, ///< starting position to seek
 uint32 /*bytes*/ ///< number of bytes  to seek
 )
{
	/// not yet impliminted
}

/*!

	@brief closes the file
*/
void IZipFile::close()
{
	delete this;
}
