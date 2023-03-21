#pragma once
#include "Core.h"

namespace ecs
{
	struct CameraComponent;
} // namespace ecs

namespace ecs::CameraModule
{
	struct DefaultCameraValuesSettings
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

	void DefaultCameraValues(CameraComponent& camera, DefaultCameraValuesSettings const& defaultSettings);
} // namespace ecs::CameraModule
