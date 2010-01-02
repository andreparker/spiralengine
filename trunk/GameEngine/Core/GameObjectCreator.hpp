#ifndef GAME_OBJECT_CREATOR_HPP
#define GAME_OBJECT_CREATOR_HPP

#include "Singleton.ipp"
#include "Log.hpp"
#include "VariableEditInfo.hpp"
#include <map>

#include <boost/function.hpp>
#include <string>
#include <vector>

namespace Spiral
{
	class GameObject;

	struct GameObjectInfo
	{
		boost::function0< Spiral::GameObject* > createCallBack;
		typedef std::vector< VariableEditInfo >::const_iterator Const_Var_Itr;
		std::vector< VariableEditInfo > variables;
	};

	class GameObjectCreator : public Singleton< GameObjectCreator >
	{
		DECL_SINGLETON( GameObjectCreator )
	public:
		GameObjectCreator(){}

		void AddDef( const std::string& className,
			         const GameObjectInfo& info )
		{
			m_creators.insert( std::pair< std::string, GameObjectInfo >(className,info) );
		}

		Spiral::GameObject* Create( const std::string& className )const;

		void GetClassNames( std::vector< std::string >& classNames )const;

		void LogClassNames()const;
		void LogClassVarInfo()const;
	private:
		typedef std::map< std::string, GameObjectInfo > ObjectDef;
		ObjectDef m_creators;
	};

	struct AutoAddGameDef
	{
		AutoAddGameDef( const std::string& className, 
			const boost::function0< Spiral::GameObject* >& createCallBack,
			const VariableEditCallBack& variableEdit )
		{
			GameObjectInfo objInfo;

			objInfo.createCallBack = createCallBack;
			variableEdit( objInfo.variables );

			GameObjectCreator::instance().AddDef( className, objInfo );
		}
	};

}
#endif