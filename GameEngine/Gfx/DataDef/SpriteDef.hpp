#ifndef SPRITE_DEF_HPP
#define SPRITE_DEF_HPP

#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>

namespace Spiral
{

namespace Data
{
	const unsigned int kSpriteDefVersion = 1;

	struct SpriteDef
	{
		SpriteDef(): imageName(), imageFileName(), textCoords(), angle( 0.0f ), width( 0.0f ), height( 0.0f ),
					 additiveBlend( false ), alphaBlend( false ), rotationOffsetX( 0.0f ), rotationOffsetY( 0.0f ){}

		template< class Archive >
		void serialize( archive& ar, const unsigned int version )
		{
			if( kSpriteDefVersion == version )
			{
				ar & BOOST_SERIALIZATION_NVP( imageName );			// image name tag
				ar & BOOST_SERIALIZATION_NVP( imageFileName );		// its png file name
				ar & BOOST_SERIALIZATION_NVP( textCoords );
				ar & BOOST_SERIALIZATION_NVP( angle );
				ar & BOOST_SERIALIZATION_NVP( width );
				ar & BOOST_SERIALIZATION_NVP( height );
				ar & BOOST_SERIALIZATION_NVP( additiveBlend );
				ar & BOOST_SERIALIZATION_NVP( alphaBlend );
				ar & BOOST_SERIALIZATION_NVP( rotationOffsetX );
				ar & BOOST_SERIALIZATION_NVP( rotationOffsetY );
			}
		}

		std::string imageName;
		std::string imageFileName;
		Rect< SpReal > textCoords;
		SpReal angle,width,height;
		bool additiveBlend,alphaBlend;
		SpReal rotationOffsetX,rotationOffsetY;
	};
}
}

BOOST_CLASS_VERSION( Spiral::Data::SpriteDef, 1 )

#endif