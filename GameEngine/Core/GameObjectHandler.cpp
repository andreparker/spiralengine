#include <algorithm>
#include <boost/bind.hpp>
#include "GameObjectHandler.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;



bool obj_comp::operator()( const boost::shared_ptr<CoreObject>& a,
						   const boost::shared_ptr<CoreObject>& b )
{
	return a->GetObjectId() < b->GetObjectId();
}


GameObjectHandler::GameObjectHandler():
m_objects()
{

}

GameObjectHandler::~GameObjectHandler()
{
	if( !m_objects.empty() )
	{
		Clear();
	}
}

void GameObjectHandler::Add( const boost::shared_ptr< CoreObject >& obj )
{
	if( obj )
	{
		m_objects.insert( obj );
	}
}

void GameObjectHandler::Remove( boost::int32_t id )
{
	Object_set::iterator itr = find_if( m_objects.begin(), m_objects.end(), boost::bind( &CoreObject::GetObjectId, _1 ) == id );

	if( itr != m_objects.end() )
	{
		m_objects.erase( itr );
	}
}

bool GameObjectHandler::Find(boost::int32_t id, boost::shared_ptr<CoreObject> &obj) const
{
	Object_set::const_iterator itr = find_if( m_objects.begin(), m_objects.end(), boost::bind( &CoreObject::GetObjectId, _1 ) == id );

	if( itr != m_objects.end() )
	{
		obj = *itr;
	}

	return bool( itr != m_objects.end() );
}

void GameObjectHandler::Clear()
{
	m_objects.clear();
}

void GameObjectHandler::DoTick( SpReal ticks )
{
	// tick the objects
	for_each( m_objects.begin(), m_objects.end(), boost::bind( &CoreObject::Tick, _1, ticks ) );
}

CoreObject* GameObjectHandler::DoClone()const
{
	return NULL;
}