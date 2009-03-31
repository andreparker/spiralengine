#ifndef FILE_HPP
#define FILE_HPP

#include "types.hpp"
#include "type_helper.ipp"

namespace gameEngine
{
	typedef enum
	{
		SK_BEGIN = 1,
		SK_CUR,
		SK_END
	} seek_e;

	/*!

		@class IFile

		@brief interface for getting input from a file
	*/
	class IFile
	{
		protected:
			IFile(){}
			virtual ~IFile(){}

		public:

			/*!

				@brief reads num bytes from a file

				@return num bytes read
			*/
			virtual uint32 read
			( 
				void* buffer, ///< pointer to a buffer to recieve data  
				uint32 count ///< number of bytes to read
			) = 0;

			/*!

				@brief gets the size of the file

				@return size of the file
			*/
			virtual uint32 size() const = 0;

			/*!

				@brief moves the read cursor
			*/
			virtual void seek
			( 
				const ENUMDEF2OBJECT( seek_e )& position, ///< starting position to seek
				uint32 bytes ///< number of bytes  to seek
			) = 0;

			/*!

				@brief closes the file
			*/
			virtual void close() = 0;

			/*!

				@brief end of file

				@return true if end of file reached
			*/
			virtual bool eof() const = 0;
	};

	/*!

		@class OFile

		@brief interface for outputing file data
	*/
	class OFile
	{
		protected:
			OFile(){}
			~OFile(){}
		public:

			/*!
				@brief writes data from buffer out to a file

				@return bytes written
			*/
			virtual uint32 write
			( 
				const void* buffer, ///< buffer containing data
				uint32 count ///< number of byte to write from buffer
			) = 0;

			/*!
				@brief closes the opened file
			*/
			virtual void close() = 0;
	};
}

#endif