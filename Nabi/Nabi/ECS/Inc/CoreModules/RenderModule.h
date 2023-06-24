#pragma once
#include "Core.h"

#include "Buffers\RenderBuffers.h"

namespace ecs::RenderModule
{
	[[nodiscard]] nabi::Rendering::Mesh CreatePrimativeCube(nabi::Context const& context, float const width, float const height, float const depth);
	[[nodiscard]] nabi::Rendering::Mesh CreatePrimativeSphere(nabi::Context const& context, int const latLines, int const longLines);
} // namespace ecs::RenderModule
