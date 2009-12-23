#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../Spiral.hpp"
#include "GuiSpriteEditor.hpp"

#include <boost/weak_ptr.hpp>

namespace Editor
{
	class App : public Spiral::Application
	{
	public:
		App();

	private:

		virtual bool DoInit( boost::int32_t argc, std::list< boost::shared_array< char > >& argList, boost::shared_ptr< Spiral::Engine >& engine );
		virtual bool DoRun( Spiral::SpReal ticks, boost::shared_ptr< Spiral::Engine >& engine );
		virtual bool DoUnInit();

		void CreateEditorButtons( const boost::shared_ptr< Spiral::GUI::GuiManager >& guiManager,const boost::shared_ptr< Spiral::Font>& font );
	private:
		boost::weak_ptr< Spiral::Engine > m_engine;
		boost::scoped_ptr< Spiral::Camera > m_camera;
		boost::shared_ptr< GuiSpriteEditor > m_spriteEditor;

	private:

	};
}

#endif