#include "Singleton.h"

#include "../Utils/DebugUtils.h"

namespace nabi::Templates
{
	namespace Singleton
	{
		void CallAssert(bool const assertCondition, std::string_view const assertMessge)
		{
			ASSERT(assertCondition, assertMessge);
		}
	} // namespace nabi::Templates::Singleton
} // namespace nabi::Templates
