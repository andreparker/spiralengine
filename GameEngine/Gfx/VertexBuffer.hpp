/*!
*/
#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

#include "../Core/Cloneable.hpp"
#include "../Core/Bindable.hpp"
#include "VertexFormatImplFwd.hpp"

namespace Spiral
{

	class Resource;
	class VertexBuffer : private boost::noncopyable, public Bindable, public Cloneable
	{
	public:
		virtual ~VertexBuffer();

		/*!
		   @function  GetType
		   @brief     returns a VertexFormat enum type
		   @return    boost::int32_t
		*/
		boost::int32_t GetType()const
		{
			return m_type;
		}

		/*!
		   @function  Create
		   @brief     creates the vertex resource for the buffer
		   @return    bool
		   @param     const VertexFormat & format - vertex format
		   @param     boost::int32_t elementSize - size of the elements/stride
		   @param     boost::int32_t vertexCount - size of the vertex buffer
		   @param     bool bManaged - manages the resource, e.g reload it if its invalid
		*/
		bool Create( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged = false );

		/*!
		   @function  GetResource
		   @brief     retrieve the resource to read/write vertex info
		   @return    boost::shared_ptr< Resource >
		*/
		boost::shared_ptr< Resource > GetResource()
		{
			return m_resource;
		}
	protected:
		VertexBuffer();

		/*!
		   @function  SetResource
		   @brief     set the vertex resource
		   @return    void
		   @param     boost::shared_ptr< Resource > & res
		*/
		void SetResource( boost::shared_ptr< Resource >& res )
		{
			m_resource = res;
		}
	private:
		boost::int32_t m_type;		///< vertex type
		boost::shared_ptr< Resource > m_resource;
	private:
		virtual bool DoCreate( const VertexFormat& format, boost::int32_t elementSize, boost::int32_t vertexCount, bool bManaged ) = 0;
	};
}
#endif