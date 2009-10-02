#ifndef SP_APP_H
#define SP_APP_H

#include <wx/wx.h>
#include <wx/app.h>
#include "spConsole.h"


namespace Spiral
{
namespace Console
{
	class SpApp : public wxApp
	{	
	public:
		SpApp();
		~SpApp();
		
		virtual bool OnInit();
		virtual int OnExit();
	private:
		SpConsole* m_console;
	};
}
}

DECLARE_APP( Spiral::Console::SpApp ) 

#endif

