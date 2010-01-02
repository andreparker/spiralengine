#include "VisualObjectCreator.hpp"
#include "VisualGameObject.hpp"
#include "../Core/Log.hpp"

using namespace Spiral;

AutoVisualDef::AutoVisualDef( const char* visualClassName, const char* gameClassName,
								const boost::function0< VisualGameObject*>& func, 
								const VariableEditCallBack& variableEdit  )
{
	VisualObjectCreator::instance().AddDef( visualClassName, gameClassName, func, variableEdit );
}

void VisualObjectCreator::AddDef( const char* visualClassName, const char* gameClassName, 
								  const boost::function0< VisualGameObject* >& func,
								  const VariableEditCallBack& variableEdit )
{
	VisualObjectInfo info;

	info.kGameObjectName = gameClassName;
	info.createFunc = func;

	variableEdit( info.variables );

	m_creators.insert( std::pair< std::string, VisualObjectInfo >( visualClassName, info ) );
}

VisualGameObject* VisualObjectCreator::CreateFromGameObjectName( const std::string& gameClassName ) const
{

	Const_Visual_Itr itr = m_creators.begin();
	VisualGameObject* visual = NULL;
	for( ; itr != m_creators.end(); ++itr )
	{
		if( (*itr).second.kGameObjectName == gameClassName )
		{
			visual = (*itr).second.createFunc();
			break;
		}
	}

	return visual;
}

void VisualObjectCreator::LogVisualObjects() const
{
	Const_Visual_Itr itr = m_creators.begin();
	for(; itr != m_creators.end(); ++itr )
	{
		LOG_D( "^wVisual Object: ^g%1% ^wAssociated Game Object -> ^g%2% \n", (*itr).first, (*itr).second.kGameObjectName );
	}
}

void VisualObjectCreator::GetVisualNames( std::vector<std::string>& visualNames ) const
{ 
	Const_Visual_Itr itr = m_creators.begin();
	for( ; itr != m_creators.end(); ++itr )
	{
		visualNames.push_back( (*itr).first );
	}
}

VisualGameObject* VisualObjectCreator::CreateFromVisualName( const std::string& visualClassName ) const
{
	Const_Visual_Itr itr = m_creators.find( visualClassName );
	VisualGameObject* visual = NULL;

	if( itr != m_creators.end() )
	{
		visual = (*itr).second.createFunc();
	}

	return visual;
}

void VisualObjectCreator::LogClassVarInfo() const
{
	Const_Visual_Itr itr = m_creators.begin();
	for(; itr != m_creators.end(); ++itr )
	{
		LOG_D( "^wVisual Object: ^g%1% ^wAssociated Game Object -> ^g%2% \n", (*itr).first, (*itr).second.kGameObjectName );
		for( VisualObjectInfo::Const_Var_Itr var_itr = (*itr).second.variables.begin();
			 var_itr != (*itr).second.variables.end(); ++var_itr )
		{
			LOG_D( "   ^wVariable Info: name ^g%1% ^woffset ^g%2% ^wsize ^g%3% \n", (*var_itr).var_name,(*var_itr).var_offset,(*var_itr).var_size );
		}
	}
}