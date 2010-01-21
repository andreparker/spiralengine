#include <boost/scope_exit.hpp>

#include "../../ThirdParty/Zip/zip.h"
#include "../../ThirdParty/zip/unzip.h"
#include "filemanager.hpp"
#include "zipfile.hpp"
#include "binaryfile.hpp"
#include "GeneralException.hpp"
#include "Log.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;


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

template<  class FileImpl, class FstreamType, class FileAbs >
bool CreateFileImpl
(
 const std::string& fileName,
 boost::shared_ptr< FileAbs >& file
)
{

	FstreamType* fs = new FstreamType( fileName.c_str(), FstreamType::binary );
	bool isOpen = fs->is_open();

	// delet the file if it did not open
	BOOST_SCOPE_EXIT( (isOpen)(fs) )
	{
		if( false == isOpen )
		{
			delete fs;
		}
	}BOOST_SCOPE_EXIT_END;


	if( isOpen )
	{
		FileImpl* newFile;
		newFile = new FileImpl( fs );
		file.reset(newFile);
	}else
	{
		LOG_I( "^ybool CreateFileImpl: ^rERROR ^wFile: ^g%1% ^rcannot be opened.\n", fileName );
	}

	return isOpen;
}
/*!
	@brief opens a *.pak file

	@return true if pak opened
*/
bool FileManager::openPack
( 
 const string& packFile ///< *.pak file name
)
{
	_zipHandle = OpenZip( packFile.c_str(), "" );
	_zipExists = bool( _zipHandle != NULL );

	if( !_zipExists )
	{
		LOG_I( "^yFileManager::openPack: ^rWARNING ^wFile: ^g%1% ^rdoes not exist or cannot be loaded.\n", packFile );
	}

	return _zipExists;
}

/*!

@brief creates a new pack file

@return true if pak created
*/
bool FileManager::createPack
( 
 const string& packFile ///< *.pak file to create
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
 const string& fileName, ///< file name
 boost::shared_ptr< OFile >& file ///< pointer to a output file object
)
{
	return CreateFileImpl< OBinaryFile, ofstream >( fileName, file );
}

/*!

	@brief creates/opens a file to read / not in *.pak file

	@return true if file created/opened
*/
bool FileManager::createFile
( 
 const string& fileName, ///< file name
 boost::shared_ptr< IFile >& file ///< pointer to a input file object
)
{
	return CreateFileImpl< IBinaryFile, ifstream >( fileName, file );
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
 const string& fileName, ///< filename
 boost::shared_ptr< IFile >& file ///< pointer to input file object
)
{
	if( !_zipCreated && _zipExists )
	{
		// try to use zip file object
		int idx;
		ZIPENTRY entry;
		if( FindZipItem( _zipHandle, fileName.c_str(), true, &idx, &entry ) == ZR_OK )
		{
			IZipFile* newZipFile;
			newZipFile = new IZipFile( _zipHandle, &entry, idx );
			file.reset(newZipFile);
			return true;
		}else // look in file dir
		{
			LOG_I( "^yFileManager::getFile: ^rWARNING ^wFile: ^g%1% ^rcannot locate file in zip, using file from directory.\n", fileName );
			return createFile( fileName, file );
		}
	}else if( !_zipCreated )
	{
		LOG_I( "^yFileManager::getFile: ^rWARNING ^wFile: ^g%1% ^rno zip, file is being loaded from file system.\n", fileName );
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
 const string& fileName ///< file name
)
{
	if( _zipCreated == false )
	{
		THROW_GENERAL_EXCEPTION( "FileManager::addFile - error, no zip created!" );
	}

	return bool( ZipAdd( _zipHandle, fileName.c_str(), fileName.c_str() ) == ZR_OK );
}