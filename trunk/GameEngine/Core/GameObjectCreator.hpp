#ifndef GAME_OBJECT_CREATOR_HPP
#define GAME_OBJECT_CREATOR_HPP

#include "Singleton.ipp"
#include "Log.hpp"
#include <map>

#include <boost/function.hpp>
#include <string>
#include <vector>

namespace Spiral
{
	class GameObject;
	class GameObjectCreator : public Singleton< GameObjectCreator >
	{
		DECL_SINGLETON( GameObjectCreator )
	public:
		GameObjectCreator(){}

		void AddDef( const std::string& className,
			         const boost::function0< Spiral::GameObject* >& createCallBack )
		{
			m_creators.insert( std::pair< std::string, boost::function0< Spiral::GameObject* > >(className,createCallBack) );
		}

		Spiral::GameObject* Create( const std::string& className )const;

		void GetClassNames( std::vector< std::string >& classNames )const;

		void LogClassNames()const;
	private:
		std::map< std::string, boost::function0< Spiral::GameObject* > > m_creators;
	};

	struct AutoAddGameDef
	{
		AutoAddGameDef( const std::string& className, 
			const boost::function0< Spiral::GameObject* >& createCallBack )
		{
			GameObjectCreator::instance().AddDef( className, createCallBack );
		}
	};

}
#endif