/*!
*/
#ifndef SIMPLE_APP_HPP
#define SIMPLE_APP_HPP

#include "../../GameEngine/Spiral.hpp"

namespace SimpleApp
{
	class App : public Spiral::Application
	{
	public:
		App();

	private:
		virtual bool DoInit( boost::int32_t argc, std::list< boost::shared_array< char > >& argList, boost::shared_ptr< Spiral::Engine >& engine );
		virtual bool DoRun( Spiral::SpReal ticks, boost::shared_ptr< Spiral::Engine >& engine );
		virtual bool DoUnInit();

		void KeyDown( const Spiral::Event& event, const boost::any& data );
	private:
		boost::shared_ptr< Spiral::Sprite > m_sprite;
		boost::shared_ptr< Spiral::Sprite > m_sprite_alpha;
		boost::shared_ptr< Spiral::EventSubscriber > m_keyDownSubscriber;
		boost::shared_ptr< Spiral::Engine > m_engine;
		boost::shared_ptr< Spiral::Font > m_arialN;
		boost::shared_ptr< Spiral::Sprite > m_fontSprite;
		Spiral::Camera* m_camera;
	};
}
#endif