#ifndef STRING_LOCALIZER_HPP
#define STRING_LOCALIZER_HPP

#include "../Core/Sp_DataTypes.hpp"

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace Spiral { namespace locale
{
	class StringLocalizer : boost::noncopyable
	{
	public:
		typedef std::map< cString, wString > string_table;
		typedef string_table::iterator               string_table_itr;
		typedef string_table::const_iterator         string_table_const_itr;

		StringLocalizer();
		~StringLocalizer();

		void SetStringDirectoryPath( const cString& dirPath )
		{
			m_stringPath = dirPath; 
		}

		bool LoadStringFile( const cString& fileName );
		void ClearStringTable();
		const wString& GetString( const cString& key )const;

	private: 
		cString MakeFullPath( const cString& fileName )const;
		void    ParseXmlTree( const boost::property_tree::ptree& tree );
	private:
		string_table m_table;
		cString      m_stringPath;
	};
} // locale
}
#endif