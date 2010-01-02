#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include "Sp_DataTypes.hpp"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

namespace Spiral
{
	class Process;
	class ProcessManager : boost::noncopyable
	{
	public:
		ProcessManager();

		void Add( const boost::shared_ptr<Process>& process );
		void Remove( const boost::shared_ptr<Process>& process );
		void Tick( SpReal ticks );
		void Clear();
	private:
		typedef std::list< boost::shared_ptr<Process> > ProcessList;
		typedef ProcessList::const_iterator Const_Process_Itr;
		typedef ProcessList::iterator Process_Itr;
		ProcessList m_processList;
	};
}
#endif