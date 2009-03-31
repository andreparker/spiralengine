#ifndef COMMON_HPP
#define COMMON_HPP

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "types.hpp"
#include "type_helper.ipp"
#include "objectmanager.hpp"
#include "coreobject.hpp"
#include "objectattribute.hpp"
#include <string.h>
#include <assert.h>

#define DEBUG_ASSERT assert

#define DEFINE_ATTR( _str, _size, _offset ) getAttributes().addAttribute( _str, _size, _offset );

#define VAR_OFFSET( var ) reinterpret_cast< int32 > ( &((Self*)0)->var )

#define APP_RTTI_DEC( _Class, _Derived )\
	public:\
	static gameEngine::common::Class_Info< _Class, _Derived > type_info;\
	static _Class& getClass()\
		{\
		 static _Class _class;\
		 return _class;\
		}\
		typedef _Derived Super;\
		typedef _Class Self;\
	_Class();\
	virtual ~_Class();\
	static void setUpAttributes();\
	protected:\
	virtual const gameEngine::int8* getClassName()const { return type_info.getClassName(); }\
	public:\
		static gameEngine::objectAttribute& getAttributes()\
		{\
			static gameEngine::objectAttribute attribute;\
			return attribute;\
		}
	

#define CLASS_RTTI_DEC( _Class, _Derived )\
	public:\
		static gameEngine::common::Class_Info< _Class, _Derived > type_info;\
		static _Class& getClass()\
		{\
			static _Class _class;\
			return _class;\
		}\
		typedef _Derived Super;\
		typedef _Class Self;\
		static _Class* createObject()\
		{\
			return new _Class;\
		}\
		static void setUpAttributes();\
	protected:\
		_Class();\
		virtual ~_Class();\
	virtual const gameEngine::int8* getClassName()const { return type_info.getClassName(); }\
	public:\
		static gameEngine::objectAttribute& getAttributes()\
		{\
			static gameEngine::objectAttribute attribute;\
			return attribute;\
		}
	

#define CLASS_RTTI_BASE( _Class )\
	public:\
		static gameEngine::common::Class_Info< _Class, gameEngine::common::null_Type > type_info;\
		static _Class& getClass()\
		{\
			static _Class _class;\
			return _class;\
		}\
		static gameEngine::common::null_Type& getSuper()\
		{\
			static gameEngine::common::null_Type _Super;\
			return _Super;\
		}\
		typedef _Class Self;\
		typedef gameEngine::common::null_Type Super;\
		static void setUpAttributes(){}\
	protected:\
		_Class();\
		virtual ~_Class();\
	virtual const gameEngine::int8* getClassName()const { return type_info.getClassName(); }\
	public:\
		static gameEngine::objectAttribute& getAttributes()\
		{\
			static gameEngine::objectAttribute attribute;\
			return attribute;\
		}

#define CLASS_RTTI_IMPL( _Class, _Derived )\
	gameEngine::common::Class_Info< _Class, _Derived > _Class::type_info( #_Class, NULL, &_Derived::getClass() );\
	void _Class::setUpAttributes()

#define CLASS_RTTI_BASE_IMPL( _Class )\
	gameEngine::common::Class_Info< _Class, gameEngine::common::null_Type > _Class::type_info( #_Class, NULL, &_Class::getSuper() );\



#define CLASS_SETUP_ATTRIBUTES( _Class, _Derived )\
		{\
			gameEngine::objectAttribute& attr0 = _Class::getAttributes();\
			gameEngine::objectAttribute& attr1 = _Derived::getAttributes();\
			_Class::setUpAttributes();\
			attr0.setDerivedAttrPtr( &attr1 );\
		}

// attr0 = attr1;\

namespace gameEngine {

	namespace common {

		extern uint32 classCount;

		struct Base_Info
		{
			virtual void SetupClassInfo() = 0;
		};

		template< class Class, class Derived >
		class Class_Info : public Base_Info
		{
			public:
				Class_Info( const int8* className, const int8* nameSpace, Derived* super ):
				  _super( super ),_className( className ),_nameSpace( NULL ),_hasDerivedClass( false )
				{
					if( nameSpace )
					{
						const size_t size = strlen( nameSpace );
						if( size )
						{
							_nameSpace = new int8[ size + 1 ];

							if( _nameSpace )
							{
								strcpy( _nameSpace, nameSpace );
							}
						}
					}

					objectManager* mgr = objectManager::instance();
					mgr->addInfo( this );

					mgr->unReference();
					//_classID = classCount++;
					//addDerivedClass( super );
				}

				~Class_Info()
				{
					delete [] _nameSpace;
					delete [] _aIsClass;
				}

				typedef Derived Super;

				// is the id specified in our castable class array
				bool isClass( uint32 classID )
				{
					bool is_class = false;
					const uint32 indx = bitIndex( classID );
					const uint32 bit  = bitInIndex( indx, classID );

					if( _aIsClass && indx <= bitIndex( _classID ) )
					{
						is_class = bool( _classID == classID || isBitOn( bit, indx ) );
					}

					return is_class;
				}

				virtual void SetupClassInfo()
				{
					addDerivedClass( getSuper() );
					CLASS_SETUP_ATTRIBUTES( Class, Derived );
				}

				bool hasDerivedClass()const { return _hasDerivedClass; }
				Derived* getSuper()const { return _super; }
				const int8* getClassName()const { return _className; }
				const int8* getNameSpace()const { return _nameSpace; }
				uint32 getClassID()const { return _classID; }

				const uint8* getClassBitArray()const { return _aIsClass; }
			protected:
				uint32 bitIndex( uint32 bitVal )const { return bitVal >> 8; }
				uint32 bitInIndex( uint32 indx, uint32 bitVal )const { return bitVal - ( indx << 8 ); }
				bool isBitOn( uint32 bit, uint32 indx )const { return bool( (_aIsClass[ indx ] & (bit << 8)) > 0 ); }
				
				bool allocateClassBitArray()
				{
					uint32 size = bitIndex( _classID ) + 1;
					_aIsClass = new uint8[ size ];

					if( _aIsClass )memset( _aIsClass, 0, size );
					return bool( _aIsClass != NULL );
				}

				void addDerivedClass( Derived* super )
				{
					_classID = ++classCount;
					const uint32 id = Super::type_info.getClassID();
					const uint32 indx = bitIndex( id );
					const uint32 bit = bitInIndex( indx, id );

					if( super && allocateClassBitArray() )
					{
						const uint8* srcArray = Derived::type_info.getClassBitArray();
						
						if( srcArray )
						{
							// transfer derived class info over to this one, we can be casted to anything our super has derived off of
							memcpy( _aIsClass, srcArray , indx + 1 );
							// add our super
							_aIsClass[ indx ] |= (1 << bit);

						}
						
						// add our self
						const uint32 idx = bitIndex( _classID );
						_aIsClass[ idx ] |= ( 1 << bitInIndex( idx, _classID ) );

						_hasDerivedClass = true;
					}

					DEBUG_ASSERT( _aIsClass != NULL );
				}

				
			protected:
				Derived* _super;
				uint32 _classID;
				uint8* _aIsClass;

				const int8* _className;
				int8* _nameSpace;
				bool _hasDerivedClass;
		};

		// dummy type
		struct null_Type
		{
			struct nullTypeInfo
			{
				nullTypeInfo():_aIsClass( NULL )
				{}
				~nullTypeInfo(){}

				uint8* _aIsClass;
				const uint8* getClassBitArray()const { return _aIsClass; }
				uint32 getClassID()const{ return 0; }
			};

			static gameEngine::objectAttribute& getAttributes()
			{
				static gameEngine::objectAttribute attribute;
				return attribute;
			}

			static void setUpAttributes()
			{
			}

			static nullTypeInfo type_info; // dummy info
		};

		template< class _t, class _f >
		_t fast_dynamic_cast( const _f* pointer )
		{
			using namespace type_helper::mpt;
			_t castedObject = NULL;

			if( pointer )
			{
				if( _f::type_info.isClass( type_utility::remove_ptr< _t >::type::type_info.getClassID() ) )
				{
					castedObject = static_cast< _t >( const_cast< _f* >( pointer ) );
				}
			}

			return castedObject;
		}
	}
}

#endif