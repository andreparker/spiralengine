#ifndef ZIPFILE_HPP
#define ZIPFILE_HPP

#include <boost/cstdint.hpp>
#include <boost/any.hpp>

#include "File.hpp"
#include "../../ThirdParty/Zip/unzip.h"

namespace Spiral
{
	/*!
		@class IZipFile

		@brief treats input source as a zip file
	*/
	class IZipFile :
		public IFile
	{
	public:
		IZipFile( HZIP handle, ZIPENTRY* entryInfo, int fileIdx );

	private:
		HZIP m_zipHandle;
		ZIPENTRY m_entryInfo;
		boost::int32_t m_fileIdx;
		bool m_isEof;

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
		virtual bool DoEof()const { return m_isEof; }

	};
}

#endif