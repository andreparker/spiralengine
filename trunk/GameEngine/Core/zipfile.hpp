#ifndef ZIPFILE_HPP
#define ZIPFILE_HPP


#include "file.hpp"
#include "Zip/unzip.h"

namespace gameEngine
{
	/*!
		@class IZipFile

		@breif treats input source as a zip file
	*/
	class IZipFile :
		public IFile
	{
		public:
			IZipFile( HZIP handle, ZIPENTRY* entryInfo, int fileIdx );
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
			virtual bool eof()const { return m_isEof; }
		protected:
			HZIP m_zipHandle;
			ZIPENTRY m_entryInfo;
			int m_fileIdx;
			bool m_isEof;

	};
}

#endif