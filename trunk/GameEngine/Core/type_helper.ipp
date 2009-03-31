#ifndef TYPE_HELPER_IPP
#define TYPE_HELPER_IPP

namespace gameEngine {

	namespace type_helper {

		#include "Templates/type_traits.hpp"
		#include "Templates/type_utility.hpp"
		#include "Templates/type_check.hpp"

	}
}

#define ENUMDEF2OBJECT( x ) gameEngine::type_helper::mpt::types::Type2Class< x >
#endif