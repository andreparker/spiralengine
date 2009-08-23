#ifndef BINARY_FILE_HPP
#define BINARY_FILE_HPP

#include "File.hpp"
#include <fstream>
#include <boost/scoped_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/any.hpp>

namespace Spiral
{
	class IBinaryFile : 
		public IFile
	{
	public:
		IBinaryFile( std::ifstream* ifs );

	private:
		boost::scoped_ptr<std::ifstream> m_istream;
		boost::uint32_t m_fileSize;

		/*!

		   @brief reads num bytes from a file

		   @return num bytes read
		*/
		virtual boost::int32_t DoRead
		( 
			boost::int8_t* buffer, ///< pointer to a buffer to recieve data  
			boost::int32_t count ///< number of bytes to read
		);

		/*!

		   @brief gets the size of the file

		   @return size of the file
		*/
		virtual boost::uint32_t DoSize() const;

		/*!

		   @brief moves the read cursor
		*/
		virtual void DoSeek
		( 
			seek_e position, ///< starting position to seek
			boost::int32_t bytes ///< number of bytes  to seek
		);

		/*!

		   @brief closes the file
		*/
		virtual void DoClose();

		/*!

		   @brief end of file

		   @return true if end of file reached
		*/
		virtual bool DoEof() const;
	};

	class OBinaryFile :
		public OFile
	{
	public:
		OBinaryFile( std::ofstream* ofs );

	private:
		boost::scoped_ptr<std::ofstream> m_ostream;

		virtual boost::int32_t DoWrite( const boost::int8_t* buffer, boost::int32_t count );
		virtual void DoClose();
	};
}

#endif