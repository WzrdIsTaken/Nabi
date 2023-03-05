#pragma once
#include "Core.h"

namespace ecs
{
	struct CameraComponent final : public nabi::ECS::ComponentSettingsBase
	{
		dx::XMFLOAT4X4 m_View;
		dx::XMFLOAT4X4 m_Projection;

		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT3 m_Target;

		bool m_HasToBeUpdated;
	};

	namespace CameraHelpers
	{
		struct DefaultCameraValuesSettings
		{
			float m_WindowWidth = 800.0f;
			float m_WindowHeight = 600.0f;

			float m_FovAngleY = dx::XM_PIDIV4;
			float m_NearZ = 0.1f;
			float m_FarZ = 100.0f;

			dx::XMFLOAT3 m_CameraViewMatrixTranslation = { 0, 0, 5 };
			dx::XMFLOAT3 m_CameraPosition = { 0, 0, 0 };
			dx::XMFLOAT3 m_CameraTarget = { 0, 0, 1 }; // Look forward
		};

		void DefaultCameraValues(CameraComponent& camera, DefaultCameraValuesSettings const& defaultSettings);
	} // namespace CameraHelpers
} // namespace ecs
