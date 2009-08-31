/*!
*/
#ifndef OGL_DRIVER_HPP
#define OGL_DRIVER_HPP

#include <list>
#include "../GfxDriver.hpp"

namespace Spiral
{

	class OglDriver : public GfxDriver
	{
	public:
		OglDriver();
		~OglDriver();

	private:
		virtual bool DoInitialize( const boost::any& data );
		virtual bool DoUnInitialize();

		virtual void DoSetState( const RenderState& state );
		virtual void DoDraw( boost::shared_ptr<Geometry>& geometry );
		virtual bool DoCreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry );
		virtual void DoBind( const boost::shared_ptr< Texture >& texture, boost::int32_t unit );
		virtual bool DoCreateTexture( const TextureInfo_t& info, boost::shared_ptr< Texture >& texture, const boost::uint8_t* data );
		virtual bool DoSetVideo( const GfxVidInfo_t& info, bool bfullscreen );
		virtual bool DoEnumerateSettings( std::list< GfxVidInfo_t >& modeList );
		virtual bool DoBeginDraw();
		virtual void DoEndDraw();
		virtual bool DoPresent();
	private:
		
		void ProcessState( const RenderState &state );

	private:
		typedef std::list< RenderState* >::iterator StateItr;
		std::list< RenderState* > m_stateList;

	};
}

#endif