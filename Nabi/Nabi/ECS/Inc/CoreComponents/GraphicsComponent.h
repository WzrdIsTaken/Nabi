#pragma once
#include "Core.h"

#include "Buffers\ConstantBuffer.h"

namespace ecs
{
	struct GraphicsComponent final : public nabi::ECS::ComponentSettingsBase
	{
		std::array<nabi::Rendering::ConstantBuffer, nabi::Rendering::ConstantBufferIndex::ENUM_COUNT> m_ConstantBuffers;
	};
} // namespace ecs
