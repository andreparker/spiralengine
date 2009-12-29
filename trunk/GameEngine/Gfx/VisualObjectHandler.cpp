#include "VisualObjectHandler.hpp"
#include "VisualGameObject.hpp"

using namespace Spiral;	

VisualObjectHandler::VisualObjectHandler( Engine* engine ):
m_pImplEngine( engine )
{

}

void VisualObjectHandler::Add( const boost::shared_ptr< VisualGameObject >& obj )
{
	GameObjectHandler::Add(obj);
}

void VisualObjectHandler::DoTick( SpReal ticks )
{
	Object_set::const_iterator itr = GetObjects().begin();
	Object_set::const_iterator itrEnd = GetObjects().end();
	for(; itr!= itrEnd; ++itr )
	{
		(*itr)->Tick( ticks );
	}

	itr = GetObjects().begin();
	VisualGameObject* objectPtr = NULL;
	for(; itr != itrEnd; ++itr )
	{
		objectPtr = static_cast<VisualGameObject*>( (*itr).get() );

		objectPtr->PreRender( m_pImplEngine );
		objectPtr->Render( m_pImplEngine, ticks );
		objectPtr->PostRender( m_pImplEngine );
	}
}

CoreObject* VisualObjectHandler::DoClone() const
{
	return NULL;
}