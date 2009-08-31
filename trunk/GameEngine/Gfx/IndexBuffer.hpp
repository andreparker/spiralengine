/*!
*/
#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include "../Core/Bindable.hpp"
#include "../Core/Cloneable.hpp"
#include "IndexFormatImplFwd.hpp"

namespace Spiral
{
	class Resource;
	class IndexBuffer : public Bindable, public Cloneable,private boost::noncopyable  
	{
	public:
		virtual ~IndexBuffer();

		/*!
		   @function  Create
		   @brief     creates the index buffer resource
		   @return    bool
		   @param     const IndexFormat & format - element size, 16,32 bits
		   @param     boost::int32_t indexCount - index count
		   @param     bool bManaged
		*/
		bool Create( const IndexFormat& format, boost::int32_t indexCount, bool bManaged = false );

		/*!
		   @function  GetResource
		   @brief     index resource data
		   @return    boost::shared_ptr< Resource >
		*/
		boost::shared_ptr< Resource > GetResource()
		{
			return m_resource;
		}

		boost::int32_t GetType()const
		{
			return m_type;
		}

		boost::int32_t GetIndexCount()const
		{
			return m_indexCount;
		}

	protected:
		IndexBuffer();

		void SetResource( boost::shared_ptr< Resource >& res )
		{
			m_resource = res;
		}

		void SetIndexType( boost::int32_t type )
		{
			m_type = type;
		}

		void SetIndexCount( boost::int32_t count )
		{
			m_indexCount = count;
		}
	private:
		boost::int32_t m_indexCount;
		boost::int32_t m_type;
		boost::shared_ptr< Resource > m_resource;

		virtual bool DoCreate( const IndexFormat& format, boost::int32_t indexCount, bool bManaged ) = 0;

	};
}
#endif