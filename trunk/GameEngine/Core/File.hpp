#ifndef FILE_HPP
#define FILE_HPP

#include <boost/utility.hpp>
#include <boost/cstdint.hpp>
#include <boost/any.hpp>

namespace Spiral
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
		boost::int32_t Read
		( 
			boost::int8_t* buffer, ///< pointer to a buffer to receive data  
			boost::int32_t count ///< number of bytes to read
		);

		/*!

			@brief gets the size of the file

			@return size of the file
		*/
		boost::uint32_t Size() const;

		/*!

			@brief moves the read cursor
		*/
		void Seek
		( 
			seek_e position, ///< starting position to seek
			boost::int32_t bytes ///< number of bytes  to seek
		);

		/*!

			@brief closes the file
		*/
		void Close();

		/*!

			@brief end of file

			@return true if end of file reached
		*/
		bool Eof() const;
	private:
		/*!

			@brief reads num bytes from a file

			@return num bytes read
		*/
		virtual boost::int32_t DoRead
		( 
			boost::int8_t* buffer, ///< pointer to a buffer to receive data  
			boost::int32_t count ///< number of bytes to read
		) = 0;

		/*!

			@brief gets the size of the file

			@return size of the file
		*/
		virtual boost::uint32_t DoSize() const = 0;

		/*!

			@brief moves the read cursor
		*/
		virtual void DoSeek
		( 
			seek_e position, ///< starting position to seek
			boost::int32_t bytes ///< number of bytes  to seek
		) = 0;

		/*!

			@brief closes the file
		*/
		virtual void DoClose() = 0;

		/*!
			@brief end of file

			@return true if end of file reached
		*/
		virtual bool DoEof() const = 0;
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
		boost::int32_t Write
		( 
			const boost::int8_t* buffer, ///< buffer containing data
			boost::int32_t count ///< number of byte to write from buffer
		);

		/*!
			@brief closes the opened file
		*/
		void Close();
	private:
		/*!
			@brief writes data from buffer out to a file

			@return bytes written
		*/
		virtual boost::int32_t DoWrite
		( 
			const boost::int8_t* buffer, ///< buffer containing data
			boost::int32_t count ///< number of byte to write from buffer
		) = 0;

		/*!
			@brief closes the opened file
		*/
		virtual void DoClose() = 0;

	};

	template< class FileType >
	class File_Auto_Close : private boost::noncopyable
	{
	public:
		explicit File_Auto_Close( boost::shared_ptr< FileType >& filePtr ):
			m_filePtr( filePtr ){}

		~File_Auto_Close()
		{
			if( m_filePtr )
			{
				m_filePtr->Close();
			}
		}
	protected:
		File_Auto_Close();

	private:
		boost::shared_ptr< FileType > m_filePtr;
	};
}

#endif