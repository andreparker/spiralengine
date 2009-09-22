#include "FontFactory.hpp"

using namespace Spiral;

FontFactory::~FontFactory()
{

}

bool FontFactory::Initialize()
{
	return DoInitialize();
}

boost::shared_ptr< Font > FontFactory::CreateFont( const boost::shared_ptr< IFile >& fontFile, boost::int32_t charWidth, boost::int32_t charHeight )
{
	return DoCreateFont( fontFile, charWidth, charHeight );
}

FontFactory::FontFactory()
{}