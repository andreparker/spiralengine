#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include "types.hpp"

namespace gameEngine{
	namespace common{

		class coreObject
		{
			public:
				virtual uint32 reference() = 0;
				virtual uint32 unReference() = 0;
				virtual void destroy() = 0;

			protected:
				coreObject(){}
				virtual ~coreObject(){}
		};
	}
}
#endif