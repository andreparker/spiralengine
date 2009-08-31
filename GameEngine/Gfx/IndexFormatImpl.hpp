/*!
*/
#ifndef INDEX_FORMAT_IMPL_HPP
#define INDEX_FORMAT_IMPL_HPP

#include <boost/cstdint.hpp>

namespace Spiral
{

namespace Impl
{
	struct IndexFormat
	{

		enum
		{
			IF_INVALID = -1,
			IF_16BIT,
			IF_32BIT
		};

		IndexFormat():
			type( IF_INVALID ){}

		static IndexFormat Create_16Bit()
		{
			return IndexFormat( IF_16BIT, 2 );
		}

		static IndexFormat Create_32Bit()
		{
			return IndexFormat( IF_32BIT, 4 );
		}

		boost::int32_t GetType()const
		{
			return type;
		}

		boost::int32_t GetSize()const
		{
			return size;
		}
	private:
		IndexFormat( boost::int32_t type_, boost::int32_t size_ ):
		   type( type_ ),size( size_ ){}
		boost::int32_t type;
		boost::int32_t size;		// size bytes
	};
}

typedef Impl::IndexFormat IndexFormat;

}

#endif