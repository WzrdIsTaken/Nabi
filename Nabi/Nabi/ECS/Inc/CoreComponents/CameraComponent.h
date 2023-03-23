#pragma once
#include "Core.h"

namespace ecs
{
	struct CameraIndex final
	{
		enum Enum
		{
			Perspective = 0u,
			Orthographic,
			ENUM_COUNT
		};
	};

	struct CameraComponent final : public nabi::ECS::ComponentBase
	{
		dx::XMFLOAT4X4 m_View;
		dx::XMFLOAT4X4 m_Projection;

		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT3 m_Target;

		bool m_HasToBeUpdated;
	};

	struct CameraGroupComponent final : public nabi::ECS::ComponentBase
	{
		std::array<CameraComponent, CameraIndex::Enum::ENUM_COUNT> m_Cameras;
	};
} // namespace ecs
