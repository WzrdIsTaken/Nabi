#include "EngineCore.h"

#include "StringStore.h"

#include "DebugUtils.h"

TSINGLETON_INSTANCE(nabi::Reflection::StringStore)

namespace nabi::Reflection
{
	StringStore::StringStore() NABI_NOEXCEPT
		: m_Store{}
	{
	}

	StringStore::~StringStore()
	{
		Clear();
	}

	void StringStore::Clear() NABI_NOEXCEPT
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_REFLECTION, 
			"Clearing the StringStore. Attempting to perform reflection related operations may now cause errors.", LOG_END);
		m_Store.clear();
	}
} // namespace nabi::Reflection
