#include "ProcessManager.hpp"
#include "Process.hpp"

using namespace Spiral;

void ProcessManager::Add( const boost::shared_ptr<Process>& process )
{
	m_processList.push_back( process );
}

void ProcessManager::Remove( const boost::shared_ptr<Process>& process )
{
	m_processList.remove( process );
}

void ProcessManager::Tick( SpReal ticks )
{
	Process_Itr itr = m_processList.begin();
	Process_Itr itrNext;

	// remove dead processes and add chained process to the list
	while( itr != m_processList.end() )
	{
		if( (*itr)->IsDead() )
		{
			itrNext = itr;
			++itrNext;
			// get the next process to chain
			if( (*itr)->GetNextProcess() )
			{
				// push this to the front to avoid processing it
				m_processList.push_front( (*itr)->GetNextProcess() );
			}

			m_processList.remove( *itr );
			itr = itrNext;
			continue;
		}

		++itr;
	}


	for( itr = m_processList.begin(); itr != m_processList.end(); ++itr )
	{
		(*itr)->OnUpdate( ticks );
	}
}

void ProcessManager::Clear()
{
	m_processList.clear();
}

ProcessManager::ProcessManager():
m_processList()
{
}