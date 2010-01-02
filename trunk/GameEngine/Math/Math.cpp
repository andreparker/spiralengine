#include "Math.hpp"
#include "../../ThirdParty/Math/Eigen/LU"

using namespace Spiral;

bool Math::UnProject( const Math::SpVector3r& winPos, const Math::SpMatrix4x4r& modelViewProj, const Rect<SpReal>& viewPort, Math::SpVector3r& unProjPos )
{
	bool unProjected = false;
	Math::SpMatrix4x4r inverseMVP;
	
	modelViewProj.computeInverse( &inverseMVP );
	Math::SpVector4r in_vec = Math::make_vector( winPos[0], winPos[1], winPos[2], 1.0f );

	in_vec[0] = ( in_vec[0] - viewPort.left ) / viewPort.right;
	in_vec[1] = ( in_vec[1] - viewPort.top  ) / viewPort.bottom;

	in_vec[0] = in_vec[0] * 2.0f - 1.0f;
	in_vec[1] = -(in_vec[1] * 2.0f - 1.0f);
	in_vec[2] = in_vec[2] * 2.0f - 1.0f;

	Math::SpVector4r out_vec = inverseMVP * in_vec;

	if( out_vec[3] != 0.0f )
	{
		unProjPos[0] = out_vec[0] / out_vec[3];
		unProjPos[1] = out_vec[1] / out_vec[3];
		unProjPos[2] = out_vec[2] / out_vec[3];
		unProjected = true;
	}

	return unProjected;
}

bool Math::Project( const SpVector3r& ProjPos,const SpMatrix4x4r& modelViewProj, const Rect<SpReal>& viewPort, SpVector3r& winPos )
{
	Math::SpVector4r in_vec = Math::make_vector( ProjPos[0], ProjPos[1], ProjPos[2], 1.0f );
	Math::SpVector4r clip_vec = modelViewProj * in_vec;
	bool result = false;

	if( clip_vec[3] != 0.0f )
	{
		// divide by perspective w -1/1
		// add 1 divide by to to translate to 0/1
		// inverse on y to get 1/0
		clip_vec[0] = ((clip_vec[0] / clip_vec[3]) + 1 )/ 2.0f;
		clip_vec[1] = 1.0f - ( ( (clip_vec[1] / clip_vec[3]) + 1 )/ 2.0f );
		//clip_vec[2] = ((clip_vec[2] / clip_vec[3]) + 1 )/ 2.0f;

		// scale to screen coordinates
		clip_vec[0] = clip_vec[0] * viewPort.right + viewPort.left;
		clip_vec[1] = clip_vec[1] * viewPort.bottom + viewPort.top;

		winPos[0] = clip_vec[0];
		winPos[1] = clip_vec[1];

		result = true;
	}

	return result;
}