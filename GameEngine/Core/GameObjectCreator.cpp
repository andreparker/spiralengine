#include "GameObjectCreator.hpp"
#include "GameObject.hpp"

using namespace Spiral;

Spiral::GameObject* GameObjectCreator::Create( const std::string& className )const
{
	std::map< std::string, boost::function0< Spiral::GameObject* > >::const_iterator itr;
	itr = m_creators.find( className );
	if( itr != m_creators.end() )
	{
		return (*itr).second();
	}

	return NULL;
}

void GameObjectCreator::GetClassNames( std::vector< std::string >& classNames ) const
{
	std::map< std::string, boost::function0< Spiral::GameObject* > >::const_iterator itr;
	
	for(itr = m_creators.begin(); itr != m_creators.end(); ++itr )
	{
		classNames.push_back( (*itr).first );
	}
}

void GameObjectCreator::LogClassNames() const
{
	std::map< std::string, boost::function0< Spiral::GameObject* > >::const_iterator itr;

	for(itr = m_creators.begin(); itr != m_creators.end(); ++itr )
	{
		LOG_D( "^wGame Object: ^g%1% \n", (*itr).first );
	}
}