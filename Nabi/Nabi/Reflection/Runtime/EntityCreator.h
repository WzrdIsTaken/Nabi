#pragma once

#include <string>
#include <unordered_map>

#include "../../Libraries/entt/entt.h"
#include "../../Templates/Singleton.h"
#include "../MetaECSTypes.h"

namespace nabi::Reflection
{
	class EntityPropertyList;
} // namespace nabi::Reflection

namespace nabi::Reflection
{
	/// <summary>
	/// Used to create entities at runtime
	/// </summary>
	class EntityCreator final : nabi::Templates::TSingleton<EntityCreator>
	{
	public:
		EntityCreator(entt::registry& registery);
		EntityCreator(entt::registry& registery, std::unordered_map<std::string, EntityTemplateData> const&& entityTemplateStore);

		void CreateEntity(std::string_view const entityTemplateName);
		void CreateEntity(std::string_view const entityTemplateName, EntityPropertyList const& entityOverriddenProperties);
		//more overloads? or a struct? but i kinda dont want to use a struct

		void AssignEntityStore(std::unordered_map<std::string, EntityTemplateData> const&& entityTemplateStore);

	private:
		std::unordered_map<std::string, EntityTemplateData> m_EntityTemplateStore; // (entity template name - template)
		entt::registry& m_Registery;
	};
} // namespace nabi::Reflection
