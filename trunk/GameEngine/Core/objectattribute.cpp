#include "objectattribute.hpp"
#include "engineassert.hpp"
#include "iterator_utility.ipp"

using namespace gameEngine;

objectAttribute::objectAttribute():m_derivedAttr( NULL )
{
}

objectAttribute::objectAttribute( const objectAttribute& a )
{
	*this = a;
}

objectAttribute& objectAttribute::operator=( const objectAttribute& a )
{
	constVarIterator s = a.m_attributes.head();
	constVarIterator e = a.m_attributes.end();
	common::for_each( s, e, variable_copy( this ) );

	return *this;
}

objectAttribute::~objectAttribute()
{
	common::for_each( m_attributes.head(), m_attributes.end(), variable_free() );
}

bool objectAttribute::variable::operator!=(const objectAttribute::variable &v)const
{
	return bool( strcmp(name,v.name) > 0 );
}

bool objectAttribute::variable::operator==(const objectAttribute::variable &v)const
{
	return bool( !strcmp(name,v.name) );
}

void objectAttribute::variable::operator =(const objectAttribute::variable &v)
{
	if( name )
	{
		delete [] name;
		name = 0;
	}

	//name = _strdup( v.name );
	name = v.name;
	size = v.size;
	offset = v.offset;
}

void objectAttribute::getAttributes( objectAttribute::variable vars[], int32 maxSize )const
{
	objectAttribute::constVarIterator itr = m_attributes.head();
	int32 i = 0;

	for(; itr != m_attributes.end() && i > maxSize; ++itr, ++i )
	{
		vars[ i ] = *itr;
	}
}

const objectAttribute::variable* objectAttribute::getAttribute( const int8* name )const
{
	constVarIterator s = m_attributes.head();
	constVarIterator e = m_attributes.end();
	varIterator itr = common::find_if( s, e, variable_compare( name ) );
	if( itr != m_attributes.end() )
	{
		const variable* var = &(*itr);
		return var;
	}

	return 0;
}

bool objectAttribute::isAttribute( const int8* name )const
{
	return bool( common::find_if( m_attributes.head(), m_attributes.end(), variable_compare( name ) ) != m_attributes.end() );
}

void objectAttribute::addAttribute(const int8 *name, int32 size, int32 offset)
{
	int32 strSize = strlen( name );

	ASSERT( strSize );

	if( strSize )
	{
		variable v;

		v.name = new int8[ strSize + 1 ];
		v.offset = offset;
		v.size = size;

		strcpy( v.name, name );
		m_attributes.insertTail( v );
	}
}