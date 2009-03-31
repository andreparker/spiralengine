// TestBed.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "..\GameEngine\Core\engineassert.hpp"
#include "..\GameEngine\Core\objectmanager.hpp"
#include "..\GameEngine\Core\type_helper.ipp"
#include "..\GameEngine\Core\object.hpp"

using namespace gameEngine;

class gameObject : public object
{
		CLASS_RTTI_DEC( gameObject, object )
	public:
		gameObject( const int8* name )
		{
			m_objectName = new int8[ strlen( name ) + 1 ];
			strcpy( m_objectName, name );
		}

		void setName( const int8* name )
		{
			if( !m_objectName )
			{
				m_objectName = new int8[ strlen( name ) + 1 ];
			}else
			{
				delete [] m_objectName;
				m_objectName = new int8[ strlen( name ) + 1 ];

			}
			strcpy( m_objectName, name );
		}
	protected:

		virtual bool initialize()
		{
			return true;
		}

		virtual void update( real tick )
		{
		}

		int window_size;
};

CLASS_RTTI_IMPL( gameObject, object )
{
	DEFINE_ATTR( "window_size", sizeof(int), VAR_OFFSET( window_size ) );
}

gameObject::gameObject()
{
}

gameObject::~gameObject()
{
}

int _tmain(int argc, _TCHAR* argv[])
{

	objectManager *mgr = objectManager::instance();

	mgr->initialize();
	gameObject* obj = gameObject::createObject();
	gameObject* child0 = gameObject::createObject();
	gameObject* child1 = gameObject::createObject();
	gameObject* child2 = gameObject::createObject();
	
	child0->setName( "object_child0" );
	child1->setName( "object_child1" );
	child2->setName( "object_child2" );

	obj->addChild( child1 );
	obj->addChild( child0 );
	obj->addChild( child2 );

	child0->unReference();
	child1->unReference();
	child2->unReference();

	if( obj )
	{
		int8 buffer[64];
		obj->logInfo( buffer, 64 );
		std::cout << buffer << std::endl;

		object* found = NULL;
		
		for( obj->find( found ); found; obj->findNext( found ) ) 
		{
			std::cout << "found object: " << found->getObjectName() << std::endl;
			found->unReference();
		}
	}

	obj->unReference();
	obj = NULL;

	mgr->destroy();
	return 0;
}

