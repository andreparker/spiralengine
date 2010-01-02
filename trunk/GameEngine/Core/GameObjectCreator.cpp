#include "GameObjectCreator.hpp"
#include "GameObject.hpp"

using namespace Spiral;

Spiral::GameObject* GameObjectCreator::Create( const std::string& className )const
{
	ObjectDef::const_iterator itr;
	itr = m_creators.find( className );
	if( itr != m_creators.end() )
	{
		return (*itr).second.createCallBack();
	}

	return NULL;
}

void GameObjectCreator::GetClassNames( std::vector< std::string >& classNames ) const
{
	ObjectDef::const_iterator itr;
	
	for(itr = m_creators.begin(); itr != m_creators.end(); ++itr )
	{
		classNames.push_back( (*itr).first );
	}
}

void GameObjectCreator::LogClassNames() const
{
	ObjectDef::const_iterator itr;

	for(itr = m_creators.begin(); itr != m_creators.end(); ++itr )
	{
		LOG_D( "^wGame Object: ^g%1% \n", (*itr).first );
	}
}

void Spiral::GameObjectCreator::LogClassVarInfo() const
{
	ObjectDef::const_iterator itr;

	for(itr = m_creators.begin(); itr != m_creators.end(); ++itr )
	{
		LOG_D( "^wGame Object: ^g%1% \n", (*itr).first );
		for( GameObjectInfo::Const_Var_Itr var_itr = (*itr).second.variables.begin();
			 var_itr != (*itr).second.variables.end(); ++var_itr )
		{
			LOG_D( "   ^wVariable Info: name ^g%1% ^woffset ^g%2% ^wsize ^g%3% \n", (*var_itr).var_name,(*var_itr).var_offset,(*var_itr).var_size );
		}
	}
}