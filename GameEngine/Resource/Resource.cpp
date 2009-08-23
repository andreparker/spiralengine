#include "Resource.hpp"
#include "ResLockImpl.hpp"
#include "../Core/Sp_DataTypes.hpp"

using namespace Spiral;
using namespace boost;

Resource::Resource()
{

}

Resource::~Resource()
{

}

bool Resource::IsValid()const
{
	return DoIsValid();
}

bool Resource::Lock(int32_t start, int32_t size, ResLockInfo_t &info, bool bDiscard /*= true*/ )
{
	return DoLock( start, size, info, bDiscard );
}

bool Resource::Lock(const Rect<boost::int32_t> &rect, ResLockRtInfo_t &info, bool bDiscard /*= true*/ )
{
	return DoLock( rect, info, bDiscard );
}

void Resource::Unlock()
{
	DoUnlock();
}

uint32_t Resource::Size()const
{
	return DoSize();
}

void Resource::Size( Rect< int32_t >& rect )const
{
	DoSize( rect );
}


ManagedResource::ManagedResource()
{
}

ManagedResource::~ManagedResource()
{
}

bool ManagedResource::Reload()
{
	return DoReload();
}