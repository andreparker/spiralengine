#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include "Singleton.ipp"
#include "File.hpp"

#include <boost/cstdint.hpp>
#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>
#include <string>

namespace Spiral
{

	class FileManager : public Singleton< FileManager >
	{
			DECL_SINGLETON( FileManager )
			FileManager();
		public:

			/*!
				@brief opens a *.pak file

				@return true if pak opened
			*/
			bool openPack
			( 
				const std::string& packFile ///< *.pak file name
			);
			
			/*!
				
				@brief creates a new pack file

				@return true if pak created
			*/
			bool createPack
			( 
				const std::string& packFile ///< *.pak file to create
			);

			/*!
				
				@brief creates/opens a file to write to / not in *.pak file

				@return true if file created/opened
			*/
			bool createFile
			( 
				const std::string& fileName, ///< file name
				boost::shared_ptr< OFile >& file ///< pointer to a output file object
			);

			/*!
				
				@brief creates/opens a file to read / not in *.pak file

				@return true if file created/opened
			*/
			bool createFile
			( 
				const std::string& fileName, ///< file name
				boost::shared_ptr< IFile >& file ///< pointer to a input file object
			);

			/*!
				@brief closes the currently opened *.pak file

				@return true if pack closes successfully
			*/
			bool closePack();

			/*!

				@brief grabs a file out of the currently opened pack

				@return true if file found/ looks in directory if not found in pack
			*/
			bool getFile
			( 
				const std::string& fileName, ///< filename
				boost::shared_ptr< IFile >& file ///< pointer to input file object
			);

			/*!

				@brief adds a file to the open pack/ works on pack create

				@return true if file added to the pack
			*/
			bool addFile
			(
				const std::string& fileName ///< file name
			);
	};
}

#endif