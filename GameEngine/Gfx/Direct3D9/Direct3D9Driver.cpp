#include "../../Core/GeneralException.hpp"
#include "Direct3D9Driver.hpp"
#include <memory.h>
#include <loki/ScopeGuard.h>

#include <D3D9Types.h>

#define D3D_SAFE_RELEASE( obj )\
	if( obj != NULL ){ obj->Release(); obj = NULL; }

namespace Spiral
{


	Direct3D9Driver::Direct3D9Driver()
		: m_focusWindow( NULL ), m_d3d9( NULL ), m_device9( NULL )
	{

	}

	Direct3D9Driver::~Direct3D9Driver()
	{
		ReleaseD3DInterfaces();
	}

	bool Direct3D9Driver::DoInitialize( const boost::any& data )
	{
		bool isInitialized = false;
		HWND windowHandle = boost::any_cast<HWND>( data );

		m_d3d9 = Direct3DCreate9( D3D_SDK_VERSION );

		if( m_d3d9 != NULL )
		{
			Loki::ScopeGuard guard = Loki::MakeObjGuard( this, &Direct3D9Driver::ReleaseD3DInterfaces );

			D3DPRESENT_PARAMETERS pp;
			InitDefaultPresentSettings( pp );
			
			HRESULT result = m_d3d9->CreateDevice
				( D3DADAPTER_DEFAULT,
				  D3DDEVTYPE_HAL,
				  windowHandle,
				  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				  &pp,
				  &m_device9
				  );

			if( result == D3D_OK )
			{
				isInitialized = true;
				m_focusWindow = windowHandle;
				guard.Dismiss();
			}
		}

		return isInitialized;
	}

	void Direct3D9Driver::InitDefaultPresentSettings( D3DPRESENT_PARAMETERS& presentSettings )
	{
		memset( &presentSettings, 0, sizeof(presentSettings) );

		presentSettings.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentSettings.hDeviceWindow = NULL;
		presentSettings.Windowed = TRUE;
		presentSettings.BackBufferFormat = D3DFMT_UNKNOWN;
		presentSettings.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	}

	bool Direct3D9Driver::DoUnInitialize()
	{
		ReleaseD3DInterfaces();

		return true;
	}

	void Direct3D9Driver::ReleaseD3DInterfaces()
	{
		D3D_SAFE_RELEASE( m_d3d9 );
		D3D_SAFE_RELEASE( m_device9 );
	}

	void Direct3D9Driver::DoGetCaps( GfxDeviceCaps& caps ) const
	{
		D3DCAPS9 d3dCaps;
		HRESULT result = m_d3d9->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps );

		if( result == D3D_OK )
		{
			caps.maxTextureWidth = d3dCaps.MaxTextureWidth;
			caps.maxTextureHeight = d3dCaps.MaxTextureHeight;
			caps.nonPowerOf2Texture = !(( d3dCaps.TextureCaps & D3DPTEXTURECAPS_POW2 ) | 
				( d3dCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL));
		}
		else
		{
			THROW_GENERAL_EXCEPTION( "Could not get device caps" );
		}

	}

	void Direct3D9Driver::InitFullScreenPresentSettings( D3DPRESENT_PARAMETERS& presentSettings, const GfxVidInfo_t& info )
	{
		D3DDISPLAYMODE dm;
		memset( &presentSettings, 0, sizeof(presentSettings) );

		presentSettings.BackBufferWidth = info.width;
		presentSettings.BackBufferHeight = info.height;

		presentSettings.BackBufferFormat = D3DFMT_A8R8G8B8;
		presentSettings.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentSettings.Windowed = FALSE;

		presentSettings.EnableAutoDepthStencil = TRUE;
		presentSettings.AutoDepthStencilFormat = D3DFMT_D24S8;

		presentSettings.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		if( FindDisplayMode( info, dm ) )
		{
			presentSettings.FullScreen_RefreshRateInHz = dm.RefreshRate;
		}else
		{
			THROW_GENERAL_EXCEPTION("Could not find display mode");
		}

	}

	bool Direct3D9Driver::FindDisplayMode( const GfxVidInfo_t& info, D3DDISPLAYMODE& display )
	{
		D3DFORMAT fmt = info.bitDepth == 32 ? D3DFMT_A8R8G8B8 : D3DFMT_R5G6B5;

		bool isFound = false;
		D3DDISPLAYMODE currentMode;
		UINT adapterCount = m_d3d9->GetAdapterModeCount( D3DADAPTER_DEFAULT, fmt );

		if( adapterCount )
		{
			HRESULT result;
			for( UINT idx = 0; idx < adapterCount; ++idx )
			{
				result = m_d3d9->EnumAdapterModes
					(
						D3DADAPTER_DEFAULT,
						fmt,
						idx,
						&currentMode
					);

				if( currentMode.Width == info.width &&
					currentMode.Height == info.height &&
					result == D3D_OK )
				{
					display = currentMode;
					isFound = true;
				}
			}
		}

		return isFound;
	}
}