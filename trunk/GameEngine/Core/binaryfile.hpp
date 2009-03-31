#ifndef BINARY_FILE_HPP
#define BINARY_FILE_HPP

#include "file.hpp"
#include <cstdio>

namespace gameEngine
{
	class IBinaryFile : 
		public IFile
	{
		public:
			IBinaryFile( FILE* fileHandle );

			/*!

				@brief reads num bytes from a file

				@return num bytes read
			*/
			virtual uint32 read
			( 
				void* buffer, ///< pointer to a buffer to recieve data  
				uint32 count ///< number of bytes to read
			);

			/*!

				@brief gets the size of the file

				@return size of the file
			*/
			virtual uint32 size() const;

			/*!

				@brief moves the read cursor
			*/
			virtual void seek
			( 
				const ENUMDEF2OBJECT( seek_e )& position, ///< starting position to seek
				uint32 bytes ///< number of bytes  to seek
			);

			/*!

				@brief closes the file
			*/
			virtual void close();

			/*!

				@brief end of file

				@return true if end of file reached
			*/
			virtual bool eof() const;
		protected:
			FILE* m_fileHandle;
			uint32 m_fileSize;
	};

	class OBinaryFile :
		public OFile
	{
		public:
			OBinaryFile( FILE* fileHandle );

			virtual uint32 write( const void* buffer, uint32 count );
			virtual void close();
		protected:
			FILE* m_fileHandle;
	};
}

#endif