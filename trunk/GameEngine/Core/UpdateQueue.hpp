#ifndef UPDATE_QUEUE_HPP
#define UPDATE_QUEUE_HPP

#include "Singleton.ipp"
#include <boost/function.hpp>
#include <stack>

namespace Spiral
{

	typedef boost::function< void() > UPDATE_CALL_BACK;
	class UpdateQueue : public Singleton< UpdateQueue >
	{
		DECL_SINGLETON( UpdateQueue )
	public:
		UpdateQueue(){}

		void Add( const UPDATE_CALL_BACK& callback );
		void Update();
	private:
		std::stack< UPDATE_CALL_BACK > m_callbacks;
	};

	typedef SingletonHandle<UpdateQueue> UpdateQueue_handle;
}

#endif