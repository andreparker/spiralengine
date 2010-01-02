#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>

#include "CoreObject.hpp"
#include "GameObjectCreator.hpp"


#define DECL_GAME_OBJECT( gameobject_, derived_ )\
	typedef derived_ super;\
	public:\
	static const char* kClassName;\
	static Spiral::GameObject* Create_instance();\
	DECL_VARIABLE_EDIT( gameobject_ )

#define IMPL_GAME_OBJECT( gameobject_ )\
	static Spiral::AutoAddGameDef Objdefine_##__LINE__( #gameobject_, boost::bind( &gameobject_::Create_instance ), boost::bind( gameobject_::fill_in_variable_info, _1 ));\
	const char* gameobject_::kClassName = #gameobject_;\
	Spiral::GameObject* gameobject_::Create_instance()\
	{\
		return new gameobject_;\
	}\
	IMPL_VARIABLE_EDIT( gameobject_ )

namespace Spiral
{
	class GameObject : public CoreObject
	{
		DECL_GAME_OBJECT( GameObject, CoreObject )
	public:
		const std::string& GetName()const
		{
			return m_objectName;
		}

		void SetName( const std::string& name )
		{
			m_objectName = name;
		}

		const boost::shared_ptr<GameObject> FindChild( const std::string& name )const;
		void AddChild( const boost::shared_ptr<GameObject>& object );

		
	protected:
		GameObject();
		virtual void DoTick( SpReal ticks );

		
	private:
		virtual Cloneable* DoClone()const;
		
		typedef std::list< boost::shared_ptr<GameObject> >::const_iterator Const_Child_Itr;
		typedef std::list< boost::shared_ptr<GameObject> >::iterator Child_Itr;
		std::list< boost::shared_ptr<GameObject> > m_children;
		GameObject* m_parent;
		std::string m_objectName;
	};
}
#endif