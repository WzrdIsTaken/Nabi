#pragma once
#include "Core.h"

namespace ecs
{
	struct LightStateComponent;

	struct DirectionalLightComponent;
	struct LightingProperties;
} // namespace ecs
namespace nabi::Rendering
{
	struct PerLightChange;
} // namespace nabi::Rendering

namespace ecs
{
	class LightingSystem final : public nabi::ECS::SystemBase
	{
	public:
		LightingSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~LightingSystem();

		void Render(/*TODO - game time?*/);

	private:
		REFLECT_PRIVATES(LightingSystem)

		void AssignLightingProperties(nabi::Rendering::PerLightChange& perLightChangeConstantBuffer, LightingProperties const& lightProperties) const;

		void OnLightCreated(entt::registry& registry, entt::entity entity) const;
		void OnLightUpdated(entt::registry& registry, entt::entity entity) const;
		void OnLightDestroyed(entt::registry& registry, entt::entity entity) const;
		LightStateComponent& GetLightStateComponent() const;
	};
} // namespace ecs
