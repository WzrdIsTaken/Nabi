#pragma once
#include "EngineCore.h"

#include "DebugUtils.h"

// Forward Declarations
namespace entt
{
	class meta_any;
}

namespace nabi::Reflection
{
	/// <summary>
	/// Store meta objects in a lookup of string name to meta object
	/// tl;dr the main reason this exists is because I want to avoid virtual inheritance for systems
	/// </summary>
	class MetaObjectLookup final
	{
	public:
		/// <summary>
		/// Retrieves the system with systemID from the system lookup map, casting it to T
		/// </summary>
		/// <typeparam name="T">- The system's type</typeparam>
		/// <param name="systemId">- The string id of the system</param>
		/// <returns>The system</returns>
		template<typename T>
		[[nodiscard]] inline T GetObject(std::string const& systemId) const NABI_NOEXCEPT
		{
			ASSERT_FATAL(m_MetaObjectLookup.find(systemId) != m_MetaObjectLookup.end(), "A system with id " + systemId + " does not exist in the lookup map!");
			ASSERT_FATAL(m_MetaObjectLookup.at(systemId).try_cast<T>(), "The system with id " + systemId + " could not be casted to T!");
			return m_MetaObjectLookup.at(systemId).cast<T>();
		}

		/// <summary>
		/// Adds a meta system to the lookup map with id systemId
		/// </summary>
		/// <param name="systemId">- The string id of the system</param> 
		/// <param name="metaSystem">- The meta system</param>
		inline void AddObject(std::string const& systemId, entt::meta_any const& metaSystem) NABI_NOEXCEPT
		{
			ASSERT(m_MetaObjectLookup.find(systemId) == m_MetaObjectLookup.end(), "The system with id " + systemId + " is already present in the lookup map!");
			m_MetaObjectLookup.emplace(systemId, metaSystem);
		}

		std::unordered_map<std::string, entt::meta_any> m_MetaObjectLookup{};
	}; // class MetaObjectLookup
} // namespace nabi::Reflection
