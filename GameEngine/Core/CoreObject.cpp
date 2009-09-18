#include "CoreObject.hpp"

using namespace Spiral;

CoreObject::CoreObject():
m_id(0)
{}

CoreObject::~CoreObject(){}

void CoreObject::Tick( SpReal ticks )
{
	DoTick( ticks );
}