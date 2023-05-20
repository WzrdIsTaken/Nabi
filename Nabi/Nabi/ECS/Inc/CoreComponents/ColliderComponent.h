#pragma once
#include "Core.h"

namespace ecs
{
	struct ColliderComponent final : public nabi::ECS::ComponentBase
	{
		// Game defined collision enums should use this as there underlying type, and reflect the enum with REFLECT_ENUM_UNDERLYING
		typedef std::uint64_t ColliderMask;

		enum class ColliderType : int
		{
			Sphere,
			Cube,

			ENUM_COUNT
		};

		ColliderType m_ColliderType;
		dx::XMFLOAT3 m_ColliderDimensions;

		ColliderMask m_Mask;

		entt::hashed_string m_OnCollisionEnterType;   // The type which the action is on
		entt::hashed_string m_OnCollisionEnterAction; // The function to call 
		entt::hashed_string m_OnCollisionExitType;
		entt::hashed_string m_OnCollisionExitAction;

		ColliderComponent()
			: m_ColliderType(ColliderType::Cube)
			, m_ColliderDimensions{ 0.0f, 0.0f, 0.0f }
			, m_Mask(~0ull)
			, m_OnCollisionEnterType("InvalidType")
			, m_OnCollisionEnterAction("InvalidAction")
			, m_OnCollisionExitType("InvalidType")
			, m_OnCollisionExitAction("InvalidAction")
		{
		}
	};
} // namespace ecs
