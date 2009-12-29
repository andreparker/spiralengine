#include "CoreObject.hpp"

using namespace Spiral;

static boost::int32_t idNext = 0;

CoreObject::CoreObject():
m_id(++idNext)
{}

CoreObject::~CoreObject(){}

void CoreObject::Tick( SpReal ticks )
{
	DoTick( ticks );
}