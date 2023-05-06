#pragma once
#include "Core.h"

namespace ecs
{
	namespace SComp
	{
		struct LightStateComponent;
	} // namespace SComp

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

		void Render(nabi::GameTime const& gameTime);

	private:
		void AssignLightingProperties(nabi::Rendering::PerLightChange& perLightChangeConstantBuffer, LightingProperties const& lightProperties) const;

		void OnLightCreated(entt::registry& registry, entt::entity entity) const;
		void OnLightUpdated(entt::registry& registry, entt::entity entity) const;
		void OnLightDestroyed(entt::registry& registry, entt::entity entity) const;
		SComp::LightStateComponent& GetLightStateComponent() const;

		REFLECT_PRIVATES(LightingSystem)
	};
} // namespace ecs
