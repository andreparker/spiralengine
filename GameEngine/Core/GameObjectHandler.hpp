/*!
*/
#ifndef GAME_OBJECT_HANDLER_HPP
#define GAME_OBJECT_HANDLER_HPP

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <set>

#include "CoreObject.hpp"

namespace Spiral
{
	struct obj_comp
	{
		bool operator()( const boost::shared_ptr< CoreObject >& a,
			             const boost::shared_ptr< CoreObject >& b );
	};

	typedef std::set< boost::shared_ptr<CoreObject>, obj_comp > Object_set;

	class GameObjectHandler : public CoreObject, public boost::noncopyable 
	{
	public:
		GameObjectHandler();
		~GameObjectHandler();

		/*!
		   @brief adds a object
		*/
		void Add
		(
			boost::shared_ptr< CoreObject >& obj ///< object to add 
		);

		/*!
		   @brief removes a object

		*/
		void Remove
		(
			boost::int32_t id ///< id
		);

		/*!

		   @brief finds a object of id

		   @return true if found
		*/
		bool Find
		( 
			boost::int32_t id, ///< id
			boost::shared_ptr< CoreObject >& obj ///< ref to returned obj
		)const;

		/*!
		   @function   Clear
		   @brief  	   clears out the object list
		   @return     void
		*/
		void Clear();

	private:
		Object_set m_objects; ///< list of game objects

	

		virtual void DoTick( real ticks );
		virtual CoreObject* DoClone()const;
	};
}
#endif