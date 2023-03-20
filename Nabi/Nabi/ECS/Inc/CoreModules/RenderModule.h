#pragma once
#include "Core.h"

#include "Drawables\RenderBuffers.h"

namespace ecs::RenderModule
{
	nabi::Rendering::Mesh CreatePrimativeCube(nabi::Context const& context, float const width, float const height, float const depth);
	nabi::Rendering::Mesh CreatePrimativeSphere(nabi::Context const& context, int const latLines, int const longLines);
} // namespace ecs::RenderModule
