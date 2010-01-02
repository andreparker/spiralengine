#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "Sp_DataTypes.hpp"
#include <boost/noncopyable.hpp>

namespace Spiral
{
	class ProcessManager;
	class Process : boost::noncopyable
	{
	public:
		virtual ~Process(){}

		void OnUpdate( SpReal ticks )
		{
			DoOnUpDate( ticks );
		}

		void Kill()
		{
			m_killProcess = true;
		}

		bool IsDead()const
		{
			return m_killProcess;
		}

		void SetNextProcess( const boost::shared_ptr<Process>& nextProcess )
		{
			m_nextProcess = nextProcess;
		}

		const boost::shared_ptr<Process>& GetNextProcess()const
		{
			return m_nextProcess;
		}

	protected:
		Process( ProcessManager* processManager ):
			 m_nextProcess(),m_killProcess(false),m_ProcessManager(processManager)
			 {}

	private:
		boost::shared_ptr<Process> m_nextProcess;
		bool                       m_killProcess;
		ProcessManager*            m_ProcessManager;


	private:
		virtual void DoOnUpDate( SpReal ticks ) = 0;
	};
}

#endif