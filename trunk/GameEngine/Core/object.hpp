#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "common.hpp"
#include "types.hpp"
#include "linkedList.ipp"
#include "iterator_utility.ipp"

namespace gameEngine {

	class object : public common::coreObject
	{
		CLASS_RTTI_BASE( object )
			public:

				class object_functor
				{
					public:
						object_functor( real currentTick ):m_tick( currentTick ){}
						~object_functor(){}
						void operator()( object* obj );

					protected:
						real m_tick;
				};

				class object_unRef_functor
				{
					public:
						void operator()( object* obj );
				};

				friend class object_functor;
				// basic object minipulation functions
				template< class T > bool getParent( T*& parent )const;
				template< class T > bool getAncestor( T*& ancestor )const;
				template< class T > bool find( T*& obj, const int8* objName = NULL );
				template< class T > bool findNext( T*& obj, const int8* objName = NULL );

				const int8* getObjectName()const { return m_objectName; }

				virtual uint32 reference();
				virtual uint32 unReference();
				virtual void destroy();

				virtual void logInfo( int8* buffer, uint32 size );
				virtual void addChild( object* obj )
				{
					obj->m_parent = this;
					obj->reference();
					m_children.insertTail( obj ); 
				}
			protected:
				virtual bool initialize(){ return true; }
				virtual bool needsUpdate()const{ return true; }
				virtual void update( real tick ){}
				virtual void unInitialize(){}
				virtual void updateChildren( real tick );
				virtual void unReferenceChildren();
			protected:

				typedef common::linkedList< object* >::iterator obj_iterator;
				object* m_parent;
				int8* m_objectName;
				uint32 m_referenceCount;
				common::linkedList< object* > m_children;
				obj_iterator m_findNextItr;
	};

	// finds a given object type with/(with out) name
	template< class T > bool object::find( T*& obj, const int8* objName )
	{
		obj_iterator itr = m_children.head();
		bool isFound = false;
		for(; itr != m_children.end(); ++itr )
		{
			T* cast = common::fast_dynamic_cast< T* >( *itr );
			if( cast )
			{
				if( !objName || !strcmp( objName, cast->getObjectName() ) )
				{
					cast->m_findNextItr = ++itr;
					obj = cast;
					cast->reference();
					isFound = true;
					break;	
				}
			}
		}

		obj = (!isFound ? 0: obj);

		return isFound;
	}

	// searches through the children of a previous search started by find
	template< class T > bool object::findNext( T*& obj, const int8* objName )
	{
		obj_iterator itr = obj->m_findNextItr;
		object* parent;
		bool isFound = false;

		if( obj->getParent( parent ) )
		{
			for(; itr != parent->m_children.end(); ++itr )
			{
				T* cast = common::fast_dynamic_cast< T* >( *itr );
				if( cast )
				{
					if( !objName || !strcmp( objName, cast->getObjectName() ) )
					{
						cast->m_findNextItr = ++itr;
						obj = cast;
						obj->reference();
						isFound = true;
						break;	
					}
				}
			}

			parent->unReference();
			if( !isFound ) obj = 0;
		}

		return isFound;
	}

	template< class T > bool object::getParent( T*& parent )const
	{
		parent = common::fast_dynamic_cast< T* >( m_parent );
		
		// add a reference
		if( parent ) parent->reference();

		return bool( parent != NULL );
	}

	template< class T > bool object::getAncestor( T*& ancestor )const
	{
		T* currentObject = NULL;
		object* itr = m_parent;
		if( m_parent )
		{
			while( itr != NULL && !currentObject )
			{
				currentObject = common::fast_dynamic_cast< T* >( itr );
				itr = itr->m_parent;
			}

			ancestor = currentObject;

			// add a reference to this object
			if( currentObject ) currentObject->reference();

		}

		return bool( currentObject != NULL );
	}
}
#endif