#ifndef GUI_LIST_HPP
#define GUI_LIST_HPP


#include "GuiWindow.hpp"

#include <list>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include "../../Core/Sp_DataTypes.hpp"

namespace Spiral
{
	class Font;
	class GfxDriver;
namespace GUI
{

	class GuiList : public GuiWindow
	{
	public:
		GuiList( const Math::Vector2f& position, const wString itemList[], boost::uint32_t itemCount,
			boost::uint32_t maxListSize, boost::uint32_t maxItemNameLength, const boost::shared_ptr< Font >& font );
		virtual ~GuiList();

		bool AddItem( const wString& item );
		bool GetItem( boost::uint32_t index, wString& item )const;
		
		void Construct( const boost::shared_ptr< GfxDriver >& gfxDriver );

		boost::int32_t GetCurrentSelection()const
		{
			return m_currentSelection;
		}

		bool NeedsConstruct()const
		{
			return m_needsConstruct;
		}
	private:
		std::list< wString > m_items;
		typedef std::list< wString >::iterator ItemItr;
		typedef std::list< wString >::const_iterator Const_ItemItr;
		boost::shared_ptr< Font > m_font;
		const boost::uint32_t m_maxItemNameLength;
		const boost::uint32_t m_maxListSize;
		bool m_needsConstruct;
		boost::int32_t m_currentSelection;
	};
}
}
#endif