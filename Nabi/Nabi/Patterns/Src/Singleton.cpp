#include "EngineCore.h"

#include "Singleton.h"

#include "DebugUtils.h"

namespace nabi::Templates
{
	namespace SingletonHelpers
	{
		void CallAssert(bool const assertCondition, std::string_view const assertMessge) NABI_NOEXCEPT
		{
			ASSERT(assertCondition, assertMessge);
		}
	} // namespace nabi::Templates::SingletonHelpers
} // namespace nabi::Templates
