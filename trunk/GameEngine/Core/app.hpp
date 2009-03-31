#ifndef APP_HPP
#define APP_HPP

#include "object.hpp"

namespace gameEngine {
	namespace frameWork {

		class coreApp : 
			public object
		{
				APP_RTTI_DEC( coreApp, object )
			public:

				coreApp( const int8* name );

				void setAppName( const int8* name );
				const int8* getAppName()const { return m_appName; }
				virtual bool initialize();
				virtual void updateApp( real tick );
				virtual void unInitialize();

				Size2 getResolution()const{ return m_size; }
			protected:
				int8* m_appName;
				Size2 m_size;
		};

		coreApp* getApplication();
	}
}

#define IMPLEMENT_APP_INSTANCE( APP_CLASS, APP_NAME )\
namespace gameEngine\
{\
	namespace frameWork\
	{\
		coreApp* getApplication()\
		{\
			return new APP_CLASS( APP_NAME );\
		}\
	}\
}


#endif