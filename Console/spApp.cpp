#include "spApp.h"

namespace Spiral
{

namespace Console
{
	SpApp::SpApp():
	m_console( NULL )
	{
	}
	
	SpApp::~SpApp()
	{
	}
	
	bool SpApp::OnInit()
	{
		m_console = new SpConsole( NULL );
		return bool( m_console != NULL );
	}

	int SpApp::OnExit()
	{
		return 0;
	}
}
}

IMPLEMENT_APP( Spiral::Console::SpApp )