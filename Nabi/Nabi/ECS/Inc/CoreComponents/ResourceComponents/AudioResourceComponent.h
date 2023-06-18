#pragma once
#include "Core.h"

#include "CoreSingletonComponents\AudioStateComponent.h"

namespace ecs::RComp
{
	struct AudioResourceComponent final : nabi::ECS::ComponentBase
	{
		REFLECTED_MAP(m_Resources, SComp::AudioStateComponent::AudioID, std::string);

		AudioResourceComponent()
			: m_Resources{ {} }
		{
		}
	};
} // namespace ecs::RComp
