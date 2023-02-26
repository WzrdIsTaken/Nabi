#pragma once
#include "Core.h"

namespace ecs
{
	class RenderSystem final : public nabi::ECS::SystemBase
	{
	public:
		using SystemBase::SystemBase;

		void Render(/*TODO - game time?*/);

	private:
		REFLECT_PRIVATES(RenderSystem)

#ifdef USE_DEBUG_UTILS
		void DebugTraceOutput(entt::entity const entity) const;

		dx::XMMATRIX m_DebugProjectionMatrix;
		dx::XMMATRIX m_DebugViewMatrix;
		dx::XMMATRIX m_DebugModelMatrix;
#endif // USE_DEBUG_UTILS
	};
} // namespace nabi
