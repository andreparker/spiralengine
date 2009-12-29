#ifndef VISUAL_GAME_OBJECT_HPP
#define VISUAL_GAME_OBJECT_HPP

#include <boost/bind.hpp>

#include "../Core/GameObject.hpp"
#include "VisualObjectCreator.hpp"

#define DECL_VISUAL_OBJECT( visualobject_, derived_ )\
	typedef derived_ super;\
	public:\
	static const char* kClassName;\
	static Spiral::VisualGameObject* Create_instance();

#define IMPL_VISUAL_OBJECT( visualobject_, gameobject_ )\
	static Spiral::AutoVisualDef visDefine( #visualobject_, #gameobject_, boost::bind( &visualobject_::Create_instance ));\
	const char* visualobject_::kClassName = #visualobject_;\
	Spiral::VisualGameObject* visualobject_::Create_instance()\
	{\
		return new visualobject_;\
	}

namespace Spiral
{
	class Engine;
	class VisualGameObject : public GameObject
	{
		DECL_VISUAL_OBJECT( VisualGameObject, GameObject )
	public:
		virtual ~VisualGameObject();

		void PreRender( Engine* engine )
		{
			DoPreRender( engine );
		}

		void Render( Engine* engine, SpReal ticks )
		{
			DoRender( engine, ticks );
		}

		void PostRender( Engine* engine )
		{
			DoPostRender( engine );
		}

		GameObject* GetGameObject()const
		{
			return m_gameObject;
		}

		void SetGameObject( GameObject* gameObject )
		{
			m_gameObject = gameObject;
		}
	protected:
		VisualGameObject();

	private:
		GameObject* m_gameObject;

		virtual void DoPreRender( Engine* engine );
		virtual void DoRender( Engine* engine, SpReal ticks );
		virtual void DoPostRender( Engine* engine );
	};
}
#endif