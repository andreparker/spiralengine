/*!
*/
#ifndef RENDER_QUEUE_HPP
#define RENDER_QUEUE_HPP


#include <boost/ptr_container/ptr_list.hpp>
#include <boost/shared_ptr.hpp>


namespace Spiral
{

    class GfxDriver;
    class RenderEntry;

    class RenderQueue : public boost::noncopyable
    {
    public:
        RenderQueue();

        /*!
           @function   AddEntry
           @brief  	   adds a render entry to the queue
           @return     void
           @param 	   RenderEntry * entry
        */
        void AddEntry( RenderEntry* entry );

        /*!
           @function   Sort
           @brief  	   sorts the queue (bool operator< )
           @return     void
           @param 	   bool bOrder - least to greatest, reverse if false
        */
        void Sort( bool bOrder = true );

        /*!
           @function   Present
           @brief  	   renders entries
           @return     void
        */
        void Present( boost::shared_ptr< GfxDriver >& gfxDriver );

        /*!
           @function   Clear
           @brief  	   clears queue
           @return     void
        */
        void Clear();

        /*!
           @function   GetEntry
           @brief  	   finds a entry with a specified id and returns it
           @return     bool - true found
           @param 	   RenderEntry * & entry
           @param 	   boost::int32_t id - 
        */
        bool GetEntry( RenderEntry*& entry, boost::int32_t id );

    protected:
		typedef boost::ptr_list< RenderEntry >::iterator EntryItr;
        boost::ptr_list< RenderEntry > m_entryList;
    };
}

#endif