#include "Core.h"

#include "CoreModules\CameraModule.h"

namespace ecs::CameraModule
{
	void DefaultCameraValues(nabi::Context const& /*context*/, ecs::CameraComponent& cameraComponent, DefaultCameraValuesSettings const& defaultSettings)
	{
		// The thought process here is that a camera system is kinda game specific, so I don't want to make a core one.
		// However, the camera component is created in the default nabi initialization so this function can be used to default it.
		// Or, reset any other camera.

		float const aspectRatio = defaultSettings.m_WindowWidth / defaultSettings.m_WindowHeight;
		dx::XMMATRIX const projMatrix = dx::XMMatrixPerspectiveFovLH(defaultSettings.m_FovAngleY, aspectRatio, defaultSettings.m_NearZ, defaultSettings.m_FarZ);

		dx::XMFLOAT3 const view = defaultSettings.m_CameraViewMatrixTranslation;
		dx::XMMATRIX const viewMatrix = dx::XMMatrixTranslation(view.x, view.y, view.z);

		dx::XMStoreFloat4x4(&cameraComponent.m_Projection, projMatrix);
		dx::XMStoreFloat4x4(&cameraComponent.m_View, viewMatrix);

		cameraComponent.m_Position = defaultSettings.m_CameraPosition;
		cameraComponent.m_Target = defaultSettings.m_CameraTarget;
	}
} // namespace ecs::CameraModule