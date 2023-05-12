#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

#include "AABB.h"
#include "DirectXUtils.h"

namespace nabi::Physics::CollisionSolvers
{
	
} // namespace nabi::Physics::CollisionSolvers

namespace nabi::Physics::AABBSolvers
{
	[[nodiscard]] AABB CreateAABBFromTopLeft(dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT;
	[[nodiscard]] AABB CreateAABBFromCenter(dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT;
	void ReassignAABBFromTopLeft(AABB& aabb, dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT;
	void ReassignAABBFromCenter(AABB& aabb, dx::XMFLOAT3 const& center, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT;

	void MakeAABBIntoSphere(AABB& aabb, float const radius) NABI_NOEXCEPT;

	[[nodiscard]] inline dx::XMFLOAT3 GetCenter(AABB const& aabb) NABI_NOEXCEPT
	{
		using namespace nabi::Utils::DirectXUtils;
		return Float3Multiply(Float3Add(aabb.m_MinExtents, aabb.m_MaxExtents), 0.5f);
	}
	[[nodiscard]] inline dx::XMFLOAT3 GetExtents(AABB const& aabb) NABI_NOEXCEPT
	{
		using namespace nabi::Utils::DirectXUtils;
		return Float3Multiply(Float3Subtract(aabb.m_MaxExtents, aabb.m_MinExtents), 0.5f);
	}

	[[nodiscard]] inline bool Intersects(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT
	{
		using namespace nabi::Utils::DirectXUtils;
		return Float3LessOrEqual(lhs.m_MinExtents, rhs.m_MaxExtents) | Float3GreaterOrEqual(lhs.m_MaxExtents, rhs.m_MinExtents);
	}
} // namespace nabi::Physics::AABBSolvers
