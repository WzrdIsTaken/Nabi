#include "Core.h"

#include "CoreSystems\LightingSystem.h"

#include "CoreComponents\LightComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreSingletonComponents\GraphicsComponent.h"
#include "CoreSingletonComponents\LightStateComponent.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(LightingSystem)
	RELFECT_SYSTEM_END(LightingSystem)

#define MANAGE_LIGHT_LISTENERS(_event, action, callback) \
	m_Context.m_Registry._event<DirectionalLightComponent>().action<&callback>(this); \
	m_Context.m_Registry._event<SpotLightComponent>().action<&callback>(this); \
	m_Context.m_Registry._event<PointLightComponent>().action<&callback>(this);

	LightingSystem::LightingSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		// Set up listeners for the creation, updating and destruction of lights
		MANAGE_LIGHT_LISTENERS(on_construct, connect, LightingSystem::OnLightCreated);
		MANAGE_LIGHT_LISTENERS(on_update, connect, LightingSystem::OnLightUpdated);
		MANAGE_LIGHT_LISTENERS(on_destroy, connect, LightingSystem::OnLightDestroyed);
		
		//m_Context.m_Registry.emplace<DirectionalLightComponent>(m_Context.m_Registry.create());
		
		//m_Context.m_Registry.emplace<ecs::DirectionalLightComponent>(m_Context.m_Registry.create());
		// CURRENT TODO - work out why this listeners arent being called
		// Also, the update event needs to be for either the *light or transform* component was changed
		// ^^ ASKED THIS IN QUESTION 

		// stuff in TestDraw
	}

	LightingSystem::~LightingSystem()
	{
		// Remove listeners for the creation, updating and destruction of lights
		MANAGE_LIGHT_LISTENERS(on_construct, disconnect, LightingSystem::OnLightCreated);
		MANAGE_LIGHT_LISTENERS(on_update, disconnect, LightingSystem::OnLightUpdated);
		MANAGE_LIGHT_LISTENERS(on_destroy, disconnect, LightingSystem::OnLightDestroyed);		
	}

	void LightingSystem::Render()
	{
		// Get the graphics entity. This stores the camera, constant buffers, etc
		entt::entity graphicEntity = m_Context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic);

		// Cache the light state component
		LightStateComponent& lightStateComponent = m_Context.m_Registry.get<LightStateComponent>(graphicEntity);

		// Check if the lights need to be updated
		if (lightStateComponent.m_UpdateLights)
		{
			// Cache the graphics component
			GraphicsComponent& graphicsComponent = m_Context.m_Registry.get<GraphicsComponent>(graphicEntity);

			// Get the light constant buffer
			nabi::Rendering::ConstantBuffer const lightConstantBuffer = 
				graphicsComponent.m_ConstantBuffers.at(nabi::Rendering::ConstantBufferIndex::PerLightChange);
			std::vector<nabi::Rendering::PerLightChange> lightConstantBufferData;

			// Calculate how large the per light constant buffer should be
			size_t const lightCount = static_cast<size_t>(lightStateComponent.m_LightCount);
			lightConstantBufferData.resize(lightCount);

			// --- Loop through all the lights ---
			int currentLightCount = 0;

			// Directional
			m_Context.m_Registry.view<TransformComponent, DirectionalLightComponent>()
				.each([&](auto const& transformComponent, auto const& directionalLightComponent)
					{
						nabi::Rendering::PerLightChange& light = lightConstantBufferData.at(currentLightCount);

						// Directional light
						light.m_Position = transformComponent.m_Position;
						light.m_Direction = directionalLightComponent.m_Direction;
						light.m_Colour = directionalLightComponent.m_Colour;
						light.m_Intensity = directionalLightComponent.m_Intensity;

						// Defaults (things a DirectionalLight doesn't have but are in the PerLightChange constant buffer)
						light.m_AttenuationRadius = 1.0f;
						light.m_SpotAngle = 1.0f;

						// Light properties
						AssignLightingProperties(light, directionalLightComponent);
						
						++currentLightCount;
					});

			/*
			// Spot
			m_Context.m_Registry.view<TransformComponent, SpotLightComponent>()
				.each([&](auto const& transformComponent, auto const& spotLightComponent)
					{
						++currentLightCount;
					});

			// Point
			m_Context.m_Registry.view<TransformComponent, PointLightComponent>()
				.each([&](auto const& transformComponent, auto const& pointLightComponent)
					{

						++currentLightCount;
					});
			*/

			ASSERT(sizeof(nabi::Rendering::PerLightChange) * lightConstantBufferData.size(),
				"The allocated space for the light constant buffer does not match the number of lights created!");

			// --- Let there be light! ---

			m_Context.m_RenderCommand->UpdateConstantBuffer(lightConstantBuffer, lightConstantBufferData.data());
			lightStateComponent.m_UpdateLights = false;
		}
	}

	void LightingSystem::AssignLightingProperties(nabi::Rendering::PerLightChange& perLightChangeConstantBuffer, LightingProperties const& lightProperties) const
	{
		perLightChangeConstantBuffer.m_AmbientIntensity = lightProperties.m_AmbientIntensity;
		perLightChangeConstantBuffer.m_DiffuseIntensity = lightProperties.m_DiffuseIntensity;
		perLightChangeConstantBuffer.m_SpecularAttenuation = lightProperties.m_SpecularAttenuation;
		perLightChangeConstantBuffer.m_SpecularIntensity = lightProperties.m_SpecularIntensity;
	}

	void LightingSystem::OnLightCreated(entt::registry& /*registry*/, entt::entity /*entity*/) const
	{
		LightStateComponent& lightStateComponent = GetLightStateComponent();

		++lightStateComponent.m_LightCount;
		lightStateComponent.m_UpdateLights = true;
	}

	void LightingSystem::OnLightUpdated(entt::registry& /*registry*/, entt::entity /*entity*/) const
	{
		LightStateComponent& lightStateComponent = GetLightStateComponent();
		lightStateComponent.m_UpdateLights = true;
	}

	void LightingSystem::OnLightDestroyed(entt::registry& /*registry*/, entt::entity /*entity*/) const
	{
		LightStateComponent& lightStateComponent = GetLightStateComponent();

		--lightStateComponent.m_LightCount;
		lightStateComponent.m_UpdateLights = true;
	}

	LightStateComponent& LightingSystem::GetLightStateComponent() const
	{
		entt::entity graphicEntity = m_Context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic);
		return m_Context.m_Registry.get<LightStateComponent>(graphicEntity);
	}
} // namespace ecs