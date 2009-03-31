#include "platform_wxConsole.hpp"
#include "platform_wxWidget.hpp"

using namespace gameEngine;

ConsoleLog::ConsoleLog():
m_flags( gameEngine::logFlag_platform | gameEngine::logFlag_system_init | gameEngine::logFlag_results ){}

bool ConsoleLog::canLog(gameEngine::int32 flags) const
{
	return bool( m_flags & flags ? true : false );
}

void ConsoleLog::log(const gameEngine::int8 *msg)
{
	platform_app& app = wxGetApp();

	app.getConsole()->WriteText( wxString( msg ) );
}
