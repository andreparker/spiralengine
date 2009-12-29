#include "GameObject.hpp"
#include <algorithm>
#include <boost/bind.hpp>

IMPL_GAME_OBJECT( Spiral::GameObject );

using namespace Spiral;

const boost::shared_ptr<GameObject> GameObject::FindChild( const std::string& name ) const
{
	Const_Child_Itr itr = std::find_if( m_children.begin(), m_children.end(), boost::bind( &GameObject::GetName, _1 ) == name );
	boost::shared_ptr<GameObject> foundObject;

	if( itr != m_children.end() )
	{
		foundObject = *itr;
	}

	return foundObject;
}

void GameObject::AddChild( const boost::shared_ptr<GameObject>& object )
{
	if( !FindChild( object->GetName() ) )
	{
		m_children.push_back( object );
		object->m_parent = this;
	}
}

void GameObject::DoTick( SpReal ticks )
{
// 	Child_Itr itr = m_children.begin();
// 
// 	while( itr++ != m_children.end() )
// 	{
// 		(*itr)->DoTick( ticks );
// 	}
}

Cloneable* GameObject::DoClone() const
{
	return NULL;
}

GameObject::GameObject()
{

}