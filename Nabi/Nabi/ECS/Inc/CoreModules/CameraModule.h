#pragma once
#include "Core.h"

#include "CoreComponents\CameraComponent.h"

namespace ecs::CameraModule
{
	struct DefaultCameraValuesSettings final
	{
		float m_WindowWidth = 800.0f;
		float m_WindowHeight = 600.0f;

		float m_FovAngleY = dx::XMConvertToRadians(45.0f);
		float m_NearZ = 0.1f;
		float m_FarZ = 100.0f;

		dx::XMFLOAT3 m_CameraViewMatrixTranslation = { 0, 0, 5 };
		dx::XMFLOAT3 m_CameraPosition = { 0, 0, 0 };
		dx::XMFLOAT3 m_CameraTarget = { 0, 0, 1 }; // Look forward
	};

	void DefaultCameraValues(nabi::Context const& context, CameraComponent& camera, DefaultCameraValuesSettings const& defaultSettings);

	[[nodiscard]] inline CameraComponent const& GetMainPerspectiveCameraComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<CameraGroupComponent>(context.m_SingletonEntites
			.at(nabi::Context::SingletonEntities::Graphic)).m_Cameras.at(CameraIndex::Perspective);
	}
	[[nodiscard]] inline CameraComponent& GetMainPerspectiveCameraComponent(nabi::Context& context)
	{
		return const_cast<ecs::CameraComponent&>(GetMainPerspectiveCameraComponent(const_cast<nabi::Context const&>(context)));
	}

	[[nodiscard]] inline CameraComponent const& GetMainOrthographicCameraComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<CameraGroupComponent>(context.m_SingletonEntites
			.at(nabi::Context::SingletonEntities::Graphic)).m_Cameras.at(CameraIndex::Orthographic);
	}
	[[nodiscard]] inline CameraComponent& GetMainOrthographicCameraComponent(nabi::Context& context)
	{
		return const_cast<ecs::CameraComponent&>(GetMainOrthographicCameraComponent(const_cast<nabi::Context const&>(context)));
	}
} // namespace ecs::CameraModule
