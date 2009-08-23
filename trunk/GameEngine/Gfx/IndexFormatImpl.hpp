/*!
*/
#ifndef INDEX_FORMAT_IMPL_HPP
#define INDEX_FORMAT_IMPL_HPP

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
			return IndexFormat( IF_16BIT );
		}

		static IndexFormat Create_32Bit()
		{
			return IndexFormat( IF_32BIT );
		}

	private:
		IndexFormat( boost::int32_t type_ ):
		   type( type_ ){}
		boost::int32_t type;
	};
}

typedef Impl::IndexFormat IndexFormat;

}

#endif