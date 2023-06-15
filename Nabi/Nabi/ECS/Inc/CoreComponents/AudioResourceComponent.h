#pragma once
#include "Core.h"

#include "CoreSingletonComponents\AudioStateComponent.h"

namespace ecs
{
#define KEY_VALUE SComp::AudioStateComponent::AudioID, std::string

	struct AudioResourceComponent final : nabi::ECS::ComponentBase
	{
		typedef std::map<KEY_VALUE> ResourceContainer;
		CREATE_CONTAINER_WRAPPER_WITH_MEMBER_DECLARATION(AudioResourceContainerWrapper, m_Resources, 
			ResourceContainer, nabi::Reflection::StringConverter::StdMapFromString<KEY_VALUE>);
	};
	
#undef KEY_VALUE
} // namespace ecs
