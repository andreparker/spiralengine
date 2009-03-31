#ifndef APP_FRAME_WORK_HPP
#define APP_FRAME_WORK_HPP

#include "../GameEngine/Core/types.hpp"
#include "../GameEngine/Core/app.hpp"

class AppFrameWork : 
	public gameEngine::frameWork::coreApp
{
		APP_RTTI_DEC( AppFrameWork, gameEngine::frameWork::coreApp )
	public:
		AppFrameWork( const gameEngine::int8* name );

		virtual bool initialize();
		virtual void updateApp( gameEngine::real tick );
		virtual void unInitialize();
};

#endif