#ifndef UPDATE_QUEUE_HPP
#define UPDATE_QUEUE_HPP

#include "Sp_DataTypes.hpp"
#include "Singleton.ipp"
#include <boost/function.hpp>
#include <list>

namespace Spiral
{

	

	typedef boost::function< void() > UPDATE_CALL_BACK;

	struct CallBackInfo
	{
		CallBackInfo( const UPDATE_CALL_BACK& update, SpReal delayTick, SpReal currentTick ):
		callback( update ), m_delayTick( delayTick ), m_currentStartTick( currentTick ){}

		UPDATE_CALL_BACK callback;
		SpReal m_delayTick;
		SpReal m_currentStartTick;
	};

	class UpdateQueue : public Singleton< UpdateQueue >
	{
		DECL_SINGLETON( UpdateQueue )
	public:
		UpdateQueue();

		void Add( const UPDATE_CALL_BACK& callback, SpReal delayTick = 0.0f );
		void Tick( SpReal tick );
	private:
		std::list< CallBackInfo > m_callbacks;
		SpReal m_ticks;

	};

	typedef SingletonHandle<UpdateQueue> UpdateQueue_handle;
}

#endif