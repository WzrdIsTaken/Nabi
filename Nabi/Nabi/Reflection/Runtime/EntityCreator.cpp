#include "EntityCreator.h"

//#include <vector>

#include "../../Utils/DebugUtils.h"
#include "EntityPropertyList.h"

TSINGLETON_INSTANCE(nabi::Reflection::EntityCreator)

namespace nabi::Reflection
{
	EntityCreator::EntityCreator(entt::registry& registery)
		: m_Registery(registery)
	{
	}

	EntityCreator::EntityCreator(entt::registry& registery, std::unordered_map<std::string, EntityTemplateData> const&& entityTemplateStore)
		: m_Registery(registery)
		, m_EntityTemplateStore(entityTemplateStore)
	{
	}

	void EntityCreator::CreateEntity(std::string_view const entityTemplateName)
	{
		//ASSERT(m_EntityTemplateStore.find(entityTemplateName.data()) != m_EntityTemplateStore.end(),
		//	"Trying to create an entity which doesn't exist in the entity store!");
	}

	void EntityCreator::CreateEntity(std::string_view const entityTemplateName, EntityPropertyList const& entityOverriddenProperties)
	{
	}

	void EntityCreator::AssignEntityStore(std::unordered_map<std::string, EntityTemplateData> const&& entityTemplateStore)
	{
		m_EntityTemplateStore = entityTemplateStore;
	}
} // namespace nabi::Reflection
