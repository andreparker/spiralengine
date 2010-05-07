#include "Profiler.hpp"
#include <algorithm>
#include <boost/foreach.hpp>
#include <fstream>

#if defined(_WIN32) && defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define GET_MICRO_SECS GetTickCount
#else
#include <ctime>
boost::uint32_t time_ex() { return (boost::uint32_t)time(NULL); }
#define GET_MICRO_SECS time_ex
#endif



namespace Util
{
	// current function name for _FindFunctionCallee
	const char* _currentFuncName = NULL;

	FunctionProfileMgr::FunctionProfileMgr()
	{

	}

	FunctionProfileMgr::~FunctionProfileMgr()
	{
		WriteToFile("DefaultProfile.log");
		ClearProfileData();
	}

	boost::uint32_t FunctionProfileMgr::push(FuncItr profileItr,const char *funcName)
	{
		boost::uint32_t stackDepth = static_cast< boost::uint32_t >( m_FunctionStack.size() );
		funcStackDef func;

		func.funcName = funcName;
		func.itr = profileItr;

		m_FunctionStack.push( func );
		return stackDepth;
	}

	void FunctionProfileMgr::End()
	{
		m_FunctionStack.pop();
	}

	bool _FindFunc( FunctionProfileData* funcPtr )
	{
		return (funcPtr->GetName() == _currentFuncName);
	}

	FunctionProfileMgr::FuncItr FunctionProfileMgr::_FindFunctionCallee( FuncItr caller, const char* funcName )
	{

		// look for the input function name
		FuncItr end   = m_FunctionProfiles.end();

		// set the function name for the search
		_currentFuncName = funcName;

		// starting from our caller, find a instance of our function
		// we search from the caller so we can track unique calls
		FuncItr item = std::find_if( caller, end, &_FindFunc );

		return item;
	}

	FunctionProfileData* FunctionProfileMgr::Begin( const char* funcName )
	{
		// the caller is the stacks top
		FuncItr caller = ( m_FunctionStack.size() ? m_FunctionStack.top().itr : m_FunctionProfiles.begin() );

		// search for a instance of the function in our list from the caller
		FuncItr profilePtr = _FindFunctionCallee(caller,funcName);

		if( profilePtr == m_FunctionProfiles.end() )
		{
			// create a new profile and insert it after the caller
			FunctionProfileData* newProfile = new FunctionProfileData( funcName );

			if( m_FunctionStack.empty() )
			{
				 m_FunctionProfiles.push_back( newProfile );
				 profilePtr = m_FunctionProfiles.begin();
			}
			else
				profilePtr = m_FunctionProfiles.insert( ++caller, newProfile );
		}

		// put the function on to the stack
		boost::uint32_t stackDepth = push( profilePtr, funcName );
		(*profilePtr)->SetStackDepth( stackDepth );

		return *profilePtr;
	}

	// write out profile data to file
	void FunctionProfileMgr::WriteToFile( const char* filename )const
	{
		std::fstream profileData( filename, std::ios::out | std::ios::trunc );

		if( profileData.is_open() )
		{
			BOOST_FOREACH( FunctionProfileData* profilePtr, m_FunctionProfiles )
			{
				profileData << "\"" << profilePtr->GetName() << "\"" << "," << profilePtr->GetStackDepth() 
					<< "," << profilePtr->GetMicrosecs() << "," << profilePtr->GetSamples() << std::endl;
			}

			profileData.close();
		}
	}

	// clear the profile data and restart
	void FunctionProfileMgr::ClearProfileData()
	{
		BOOST_FOREACH( FunctionProfileData* profilePtr, m_FunctionProfiles )
		{
			delete profilePtr;
		}

		m_FunctionProfiles.clear();
	}

	FunctionProfile::FunctionProfile( const char* functionName ):
	m_functionName( functionName )
	{
		m_ProfileDataRef = FunctionProfileMgr::singleton().Begin( functionName );
		m_ProfileDataRef->begin();
	}

	FunctionProfile::~FunctionProfile()
	{
		m_ProfileDataRef->end();
		FunctionProfileMgr::singleton().End();
	}

	void FunctionProfileData::begin()
	{
		m_microsecTmp = GET_MICRO_SECS();
	}

	void FunctionProfileData::end()
	{
		m_microsec += GET_MICRO_SECS() - m_microsecTmp;
		++m_samples;
	}

}