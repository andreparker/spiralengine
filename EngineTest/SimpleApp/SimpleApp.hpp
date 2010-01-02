/*!
*/
#ifndef SIMPLE_APP_HPP
#define SIMPLE_APP_HPP

#include "../../GameEngine/Spiral.hpp"
#include <boost/weak_ptr.hpp>

namespace SimpleApp
{
	class TestGameObject : public Spiral::GameObject
	{
		DECL_GAME_OBJECT( SimpleApp::TestGameObject, Spiral::GameObject )
	public:
		TestGameObject(){}

	private:
		int m_test_var;
		int m_test_var2[100];
	};

	class TestVisualObject : public Spiral::VisualGameObject
	{
		DECL_VISUAL_OBJECT( SimpleApp::TestVisualObject, Spiral::VisualGameObject )
	public:
		TestVisualObject(){}
	private:
		int m_test_var1;
	};

	class App : public Spiral::Application
	{
	public:
		App();

	private:
		virtual bool DoInit( boost::int32_t argc, std::list< boost::shared_array< char > >& argList, boost::shared_ptr< Spiral::Engine >& engine );
		virtual bool DoRun( Spiral::SpReal ticks, boost::shared_ptr< Spiral::Engine >& engine );
		virtual bool DoUnInit();

		void KeyDown( const Spiral::Event& event, const boost::any& data );
		void ButtonPress( boost::int32_t eventId, Spiral::GUI::GuiWindow* window, const boost::any& data );
		void SliderChanged( boost::int32_t eventId, Spiral::GUI::GuiWindow* window, const boost::any& data );
	private:
		boost::shared_ptr< Spiral::Sprite > m_sprite;
		boost::shared_ptr< Spiral::Sprite > m_sprite_alpha;
		boost::shared_ptr< Spiral::EventSubscriber > m_keyDownSubscriber;
		boost::weak_ptr< Spiral::Engine > m_engine;
		boost::shared_ptr< Spiral::Font > m_arialN;
		boost::shared_ptr< Spiral::GUI::GuiFrame > m_window;
		Spiral::Camera* m_camera;
		boost::uint32_t m_sliderEditId;
		boost::shared_ptr< Spiral::Audio::AudioStreamObject > m_testSong;
		boost::shared_ptr< Spiral::IFile > m_OggStreamFile;

		boost::shared_ptr<TestGameObject> m_gTest;
	};
}
#endif