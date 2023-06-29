#include "EngineCore.h"

#include "StringStore.h"

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

	std::string const& StringStore::Add(std::string_view const string) NABI_NOEXCEPT
	{
		return m_Store.emplace_back(string);
	}

	void StringStore::Clear() NABI_NOEXCEPT
	{
		m_Store.clear();
	}
} // namespace nabi::Reflection
