#include "DeviceDriver.hpp"

using namespace Spiral;

bool DeviceDriver::Initialize(const boost::any& data /* = boost::any */)
{
	return DoInitialize( data );
}

bool DeviceDriver::UnInitialize()
{
	return DoUnInitialize();
}