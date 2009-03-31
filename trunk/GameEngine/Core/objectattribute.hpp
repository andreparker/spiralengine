#ifndef OBJECT_ATTRIBUTE_HPP
#define OBJECT_ATTRIBUTE_HPP

#include "types.hpp"
#include "linkedList.ipp"
#include <string.h>

namespace gameEngine
{
	class objectAttribute
	{
		public:
			objectAttribute();
			objectAttribute( const objectAttribute& a );
			~objectAttribute();

			struct variable
			{
				void operator=(const variable& v);
				bool operator!=(const variable& v)const;
				bool operator==(const variable& v)const;
				variable():name(0),size(0),offset(0){}
				int8* name;
				int32 size;			// its size in bytes
				int32 offset;		// were the variable exist in memory from the objects address
			};

			objectAttribute& operator=( const objectAttribute& a );
			class variable_compare
			{
				public:
					variable_compare( const int8* str ):cmpStr( str ){}

					bool operator()( const variable& v )const
					{
						return bool( !strcmp( v.name, cmpStr ) );
					}

				protected:
					const int8* cmpStr;
			};

			class variable_free
			{
				public:
					void operator()( const variable& v )const
					{
						delete [] v.name;
					}
			};

			class variable_copy
			{
				public:
					variable_copy( objectAttribute* obj ):m_attribute(obj){}

					void operator()( const variable& v )
					{
						variable newVar;
						newVar.name = strdup( v.name );
						newVar.offset = v.offset;
						newVar.size = v.size;
						m_attribute->m_attributes.insertTail( newVar );
					}

				protected:
					objectAttribute* m_attribute;

			};

			friend class variable_copy;

			void addAttribute( const int8* name, int32 size, int32 offset );
			bool isAttribute( const int8* name )const;
			const variable* getAttribute( const int8* name )const;
			void getAttributes( variable vars[], int32 maxSize )const;

			const objectAttribute* getDeivedAttrPtr()const { return m_derivedAttr; }
			void setDerivedAttrPtr( objectAttribute* ptr ) { m_derivedAttr = ptr; }
		protected:
			typedef common::linkedList< variable > varListType;
			typedef common::linkedList< variable >::iterator varIterator;
			typedef common::linkedList< variable >::const_iterator constVarIterator;
			common::linkedList< variable > m_attributes;

			objectAttribute* m_derivedAttr; ///< derived attribute class
	};
}

#endif