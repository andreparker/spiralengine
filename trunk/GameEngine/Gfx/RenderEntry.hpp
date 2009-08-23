/*!
*/
#ifndef RENDER_ENTRY_HPP
#define RENDER_ENTRY_HPP

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include "../Core/Sp_DataTypes.hpp"

namespace Spiral
{

    class GfxDriver;
    class RenderEntry
    {
    public:
		virtual ~RenderEntry();
        
		/*!
		   @function  Render
		   @brief     performs custom rendering
		   @return    void
		   @param     boost::shared_ptr< GfxDriver > & gfxDriver
		*/
		void Render( boost::shared_ptr< GfxDriver >& gfxDriver );

        bool hasAlpha()const
        {
            return m_bAlpha;
        }

		boost::int32_t GetId()const
		{
			return m_id;
		}

		real GetZOrder()const
		{
			return m_zOrder;
		}

		bool operator<( const RenderEntry& entry1 )const;
		bool operator>( const RenderEntry& entry1 )const;

	protected:
		RenderEntry();
		RenderEntry( bool bhasAlpha, boost::int32_t id );

    private:
		virtual void DoRender( boost::shared_ptr< GfxDriver >& gfxDriver ) = 0;


        bool m_bAlpha;
		boost::int32_t m_id;
		real m_zOrder;
    };

}

#endif