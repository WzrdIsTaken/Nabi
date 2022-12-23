#include "Singleton.h"

#include "../Utils/DebugUtils.h"

namespace nabi::Templates
{
	namespace SingletonHelpers
	{
		void CallAssert(bool const assertCondition, std::string_view const assertMessge)
		{
			ASSERT(assertCondition, assertMessge);
		}
	} // namespace nabi::Templates::SingletonHelpers
} // namespace nabi::Templates
