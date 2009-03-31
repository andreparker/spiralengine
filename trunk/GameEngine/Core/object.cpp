#include "object.hpp"
#include <cstdio>

using namespace gameEngine;

CLASS_RTTI_BASE_IMPL( object )

object::object():m_parent( NULL ),m_objectName( NULL ),m_referenceCount(0)
{
}

object::~object()
{
}

uint32 object::reference()
{
	return ++m_referenceCount;
}

uint32 object::unReference()
{
	uint32 count;
	if( m_referenceCount )
	{
		count = --m_referenceCount;
	}
	else
	{
		unReferenceChildren();
		delete this;
		count = 0;
	}

	return count;
}

void object::unReferenceChildren()
{
	common::for_each( m_children.head(), m_children.end(), object_unRef_functor() );
}

void object::destroy()
{
	m_referenceCount = 0;
	delete this;
}

void object::object_functor::operator ()( object* obj )
{
	if( obj->needsUpdate() )
	{
		obj->update( m_tick );
	}

	common::for_each( obj->m_children.head(), obj->m_children.end(), object_functor( m_tick ) );
}

void object::object_unRef_functor::operator ()( object* obj )
{
	common::for_each( obj->m_children.head(), obj->m_children.end(), object_unRef_functor() );
	obj->unReference();
}

void object::updateChildren( real tick )
{
	common::for_each( m_children.head(), m_children.end(), object_functor( tick ) );
}

void object::logInfo( int8* buffer, uint32 size )
{
	sprintf( buffer, "[+ object: %s]\nclass: %s\n[- object:]\n", getObjectName(), getClassName() ); 
}