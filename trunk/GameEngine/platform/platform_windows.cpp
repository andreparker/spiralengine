#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE

#include "platform_windows.hpp"

using namespace gameEngine;

DWORD _tickCount = 0;

inline real getSeconds( DWORD tick )
{
	return static_cast< real >( tick ) / 1000.0f;
}

AppWindow::~AppWindow()
{
}

AppWindow::AppWindow( frameWork::coreApp* app )
{
	Bit32 flags;
	flags.Set( CF_OVERLAPPEDWINDOW );

	if( app )
	{
		Create( app->getAppName() ,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, flags );

	}else
	{
		Create( "gameEngine Window",CW_USEDEFAULT, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, flags );
	}
}

void AppWindow::OnDestroy()
{
	PostQuitMessage( 0 );
	m_isQuiting = true;
	Window32::OnDestroy();
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{

	objectManager* objectMgr = objectManager::instance();
	objectMgr->initialize();
	frameWork::coreApp* app = frameWork::getApplication();
	AppWindow* appWindow = new AppWindow( app );

	_tickCount = GetTickCount();

	if( appWindow && appWindow->Created() )
	{
		// show our window
		appWindow->Show();

		while( appWindow != NULL && !appWindow->isQuiting() )
		{
			if( !appWindow->PeekMessages() )
			{
				DWORD tick = GetTickCount() - _tickCount;
				if( app )
				{
					float fracSeconds = getSeconds( tick ); 
					app->updateApp( fracSeconds );
				}

				if( tick >= 1000 )
				{
					_tickCount = GetTickCount();
				}
			}
		}

	}


	if( app )
	{
		app->unInitialize();
		app->unReference();
	}

	appWindow->Close();
	delete appWindow;
	objectMgr->destroy();
	return 0;
}
