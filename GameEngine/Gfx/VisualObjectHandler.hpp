#ifndef VISUAL_OBJECT_HANDLER_HPP
#define VISUAL_OBJECT_HANDLER_HPP

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include "../Core/GameObjectHandler.hpp"

namespace Spiral
{
	class VisualGameObject;
	class Engine;
	class VisualObjectHandler : public GameObjectHandler
	{
	public:
		VisualObjectHandler( Engine* engine );
 
		void Add( const boost::shared_ptr< VisualGameObject >& obj );

	private:
		virtual void DoTick( SpReal ticks );
		virtual CoreObject* DoClone()const;

		Engine* m_pImplEngine;
	};
}
#endif