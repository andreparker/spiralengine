#include "zip/zip.h"
#include "zip/unzip.h"
#include "filemanager.hpp"
#include "zipfile.hpp"
#include "binaryfile.hpp"

using namespace gameEngine;


namespace
{
	HZIP _zipHandle;
	bool _zipExists;
	bool _zipCreated;
}

FileManager::FileManager()
{
	_zipExists = false;
	_zipCreated = false;
	_zipHandle = NULL;
}

/*!
@brief opens a *.pak file

@return true if pak opened
*/
bool FileManager::openPack
( 
 const std::string& packFile ///< *.pak file name
)
{
	_zipHandle = OpenZip( packFile.c_str(), "" );
	_zipExists = bool( _zipHandle != NULL );

	return _zipExists;
}

/*!

@brief creates a new pack file

@return true if pak created
*/
bool FileManager::createPack
( 
 const std::string& packFile ///< *.pak file to create
)
{
	_zipHandle = CreateZip( packFile.c_str(), "" );
	_zipCreated = bool(_zipHandle != NULL );

	return _zipCreated;
}

/*!

@brief creates/opens a file to write to / not in *.pak file

@return true if file created/opened
*/
bool FileManager::createFile
( 
 const std::string& fileName, ///< file name
 OFile*& file ///< pointer to a output file object
)
{
	FILE* hFile = fopen( fileName.c_str(), "wb" );
	if( hFile )
	{
		file = new OBinaryFile( hFile );
	}

	return bool( hFile != NULL );
}

/*!

@brief creates/opens a file to read / not in *.pak file

@return true if file created/opened
*/
bool FileManager::createFile
( 
 const std::string& fileName, ///< file name
 IFile*& file ///< pointer to a input file object
)
{
	FILE* hFile = fopen( fileName.c_str(), "rb" );
	if( hFile )
	{
		file = new IBinaryFile( hFile );
	}

	return bool( hFile != NULL );
}

/*!
@brief closes the currently opened *.pak file

@return true if pack closes successfully
*/
bool FileManager::closePack()
{
	if( _zipExists || _zipCreated )
	{
		CloseZip( _zipHandle );
		_zipHandle = NULL;
		_zipExists = false;
		_zipCreated = false;
	}

	return true;
}

/*!

@brief grabs a file out of the currently opened pack

@return true if file found/ looks in directory if not found in pack
*/
bool FileManager::getFile
( 
 const std::string& fileName, ///< filename
 IFile*& file ///< pointer to input file object
)
{
	if( !_zipCreated && _zipExists )
	{
		// try to use zip file object
		int idx;
		ZIPENTRY entry;
		if( FindZipItem( _zipHandle, fileName.c_str(), true, &idx, &entry ) == ZR_OK )
		{
			file = new IZipFile( _zipHandle, &entry, idx );
			return true;
		}else // look in file dir
		{
			return createFile( fileName, file );
		}
	}else if( !_zipCreated )
	{
		// use binary file object
		return createFile( fileName, file );
	}

	return false;
}

/*!

@brief adds a file to the open pack/ works on pack create

@return true if file added to the pack
*/
bool FileManager::addFile
(
 const std::string& fileName ///< file name
)
{
	return bool( ZipAdd( _zipHandle, fileName.c_str(), fileName.c_str() ) == ZR_OK );
}