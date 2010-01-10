#include "GuiList.hpp"
#include "../Font.hpp"
#include "../GfxDriver.hpp"
#include "../Surface.hpp"

using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;

bool GuiList::AddItem( const SpString& item )
{
	bool itemAdded = false;
	if( item.length() <= m_maxItemNameLength &&
		m_items.size() < m_maxListSize )
	{
		m_items.push_back( item );
		m_needsConstruct = true;	// need to rebuild list texture and window
		itemAdded = true;
	}

	return itemAdded;
}

GuiList::GuiList( const Math::Vector2f& position, const SpString itemList[], boost::uint32_t itemCount,
				 boost::uint32_t maxListSize, boost::uint32_t maxItemNameLength, const boost::shared_ptr< Font >& font ):
GuiWindow( position, Rect< SpReal >(), Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), shared_ptr<Texture>(), false ),
m_items(),
m_font( font ),
m_maxItemNameLength( maxItemNameLength ),
m_maxListSize( maxListSize ),
m_needsConstruct(true),
m_currentSelection(-1)
{
	Show( false );
	if( itemCount > 0 )
	{
		do 
		{
			AddItem( *itemList );
			++itemList;
		} while ( --itemCount );
	}
}

GuiList::~GuiList()
{
}

void GuiList::Construct( const boost::shared_ptr< GfxDriver >& gfxDriver )
{

}

bool GuiList::GetItem( boost::uint32_t index, SpString& item )const
{
	bool result = false;
	if( index < m_items.size() )
	{
		Const_ItemItr itr = m_items.begin();
		Const_ItemItr end = m_items.end();
		while( index && itr != end )
		{
			++itr;
			--index;
		}

		if( itr != end )
		{
			item = *itr;
			result = true;
		}
		
	}

	return result;
}