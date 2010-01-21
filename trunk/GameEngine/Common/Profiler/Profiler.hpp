#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <vector>
#include <list>
#include <stack>
#include <boost/cstdint.hpp>


namespace Util
{
	class FunctionProfile;

	class FunctionProfileData
	{
		public:
			FunctionProfileData(const char* functionName ): 
			  m_functionName( functionName ),m_samples(0),m_stackDepth(0),
			  m_microsec(0),m_microsecTmp(0){}

			// record the time
			void begin();
			void end();

			const char* GetName()const
			{
				return m_functionName;
			}

			void SetStackDepth( boost::uint32_t depth ) { m_stackDepth = depth; }
			boost::uint32_t GetStackDepth()const{ return m_stackDepth; }
			boost::uint32_t GetSamples()const{ return m_samples; }
			boost::uint32_t GetMicrosecs()const{ return m_microsec; }

		private:
			const char* m_functionName;
			boost::uint32_t m_stackDepth;
			boost::uint32_t m_samples;
			boost::uint32_t m_microsec;
			boost::uint32_t m_microsecTmp;
	};

	

	class FunctionProfileMgr
	{
			FunctionProfileMgr();
			FunctionProfileMgr(const FunctionProfileMgr& );
			FunctionProfileMgr& operator=(const FunctionProfileMgr& );

			typedef std::list< FunctionProfileData* >::iterator FuncItr;
		public:

			~FunctionProfileMgr();

			static FunctionProfileMgr& singleton()
			{
				static FunctionProfileMgr _instance;
				return _instance;
			}

			// write out profile data to file
			void WriteToFile( const char* filename )const;

			// clear the profile data and restart
			void ClearProfileData();

			// call this in the constructor to record function tick data
			FunctionProfileData* Begin( const char* funcName );

			// this must be called in the profiles destructor
			void End();

		
		protected:
			
			// push a function name onto the stack
			boost::uint32_t push( FuncItr profileItr, const char* funcName );

			struct funcStackDef
			{
				const char* funcName;
				FuncItr itr;		// a iterator to the function profile for easy insertion of new profiles
			};
			

			FuncItr _FindFunctionCallee( FuncItr caller, const char* funcName );

			std::list< FunctionProfileData* > m_FunctionProfiles;
			std::stack< funcStackDef > m_FunctionStack;
	};

	class FunctionProfile
	{
		public:
			explicit FunctionProfile( const char* functionName );
			~FunctionProfile();

		protected:
			const char* m_functionName;
			FunctionProfileData* m_ProfileDataRef;
	};
}

#if defined(FUNCTION_PROFILING)
#define PROFILE_FUNCTION() Util::FunctionProfile functionProfile( __FUNCTION__ )
#define PROFILE_WRITE_DATA() Util::FunctionProfileMgr::singleton().WriteToFile("StdProfileData.data")
#else
#define PROFILE_FUNCTION()
#define PROFILE_WRITE_DATA()
#endif

#endif