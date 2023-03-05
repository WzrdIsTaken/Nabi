#pragma once
#include "Core.h"

namespace ecs
{
	class RenderSystem2D : public nabi::ECS::SystemBase
	{
	public:
		using SystemBase::SystemBase;

		void Render(/*TODO - game time?*/);

	private:
		REFLECT_PRIVATES(RenderSystem2D)
	};
} // namespace ecs
