#ifndef VISUAL_OBJECT_CREATOR_HPP
#define VISUAL_OBJECT_CREATOR_HPP

#include <map>
#include <vector>
#include <string>
#include <boost/function.hpp>

#include "../Core/Singleton.ipp"


namespace Spiral
{

	class VisualGameObject;
	struct VisualObjectInfo
	{
		const char* kGameObjectName;
		boost::function0< VisualGameObject* > createFunc;
	};

	class VisualObjectCreator : public Singleton< VisualObjectCreator >
	{
		DECL_SINGLETON( VisualObjectCreator )
	public:

		void AddDef( const char* visualClassName, const char* gameClassName, 
				     const boost::function0< VisualGameObject* >& func );

		VisualGameObject* CreateFromGameObjectName( const std::string& gameClassName )const;
		VisualGameObject* CreateFromVisualName( const std::string& visualClassName )const;

		void GetVisualNames( std::vector<std::string>& visualNames )const;
		void LogVisualObjects()const;
	private:
		typedef std::map< std::string, VisualObjectInfo >::const_iterator Const_Visual_Itr;
		typedef std::map< std::string, VisualObjectInfo >::iterator Visual_Itr;
		std::map< std::string, VisualObjectInfo > m_creators;
	};

	struct AutoVisualDef
	{
		AutoVisualDef( const char* visualClassName, const char* gameClassName,
			const boost::function0< VisualGameObject*>& func );
	};
}
#endif