#include "Bindable.hpp"

using namespace Spiral;

Bindable::~Bindable()
{}

Bindable::Bindable():
m_isBound(false)
{
}

void Bindable::Bind()
{
	DoBind();
}

void Bindable::UnBind()
{
	DoUnBind();
}