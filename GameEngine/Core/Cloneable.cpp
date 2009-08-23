#include "Cloneable.hpp"

using namespace Spiral;

boost::shared_ptr< Cloneable > Cloneable::Clone()const
{
	boost::shared_ptr< Cloneable > newClone( DoClone() );
	return newClone;
}

Cloneable::Cloneable(){}
Cloneable::~Cloneable(){}