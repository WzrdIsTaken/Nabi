#pragma once
#include "Core.h"

namespace ecs
{
	struct CameraComponent
	{
		dx::XMFLOAT4X4 m_View;
		dx::XMFLOAT4X4 m_Projection;

		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT3 m_Target;

		bool m_HasToBeUpdated;
	};
} // namespace ecs
