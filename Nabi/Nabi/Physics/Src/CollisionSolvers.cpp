#include "EngineCore.h"
#include "WinCore.h" // Undefs for min / max

#include "CollisionSolvers.h"

namespace nabi::Physics::CollisionSolvers
{
	AABB CreateAABBFromTopLeft(dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT
	{
		AABB aabb;
		ReassignAABBFromTopLeft(aabb, topLeft, dimensions);

		return aabb;
	}

	AABB CreateAABBFromCenter(dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT
	{
		AABB aabb;
		ReassignAABBFromCenter(aabb, topLeft, dimensions);

		return aabb;
	}

	void ReassignAABBFromTopLeft(AABB& aabb, dx::XMFLOAT3 const& topLeft, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT
	{
		using namespace nabi::Utils::DirectXUtils;

		dx::XMFLOAT3 const minExtents = Float3Subtract(topLeft, dimensions);

		aabb.m_MinExtents = minExtents;
		aabb.m_MaxExtents = topLeft;
	}

	void ReassignAABBFromCenter(AABB& aabb, dx::XMFLOAT3 const& center, dx::XMFLOAT3 const& dimensions) NABI_NOEXCEPT
	{
		using namespace nabi::Utils::DirectXUtils;

		dx::XMFLOAT3 const halfDimensions = Float3Divide(dimensions, 2.0f);
		dx::XMFLOAT3 const maxExtents = Float3Add(center, halfDimensions);
		dx::XMFLOAT3 const minExtents = Float3Subtract(center, halfDimensions);

		aabb.m_MinExtents = minExtents;
		aabb.m_MaxExtents = maxExtents;
	}

	void MakeAABBIntoSphere(AABB& aabb, float const radius) NABI_NOEXCEPT
	{
		using namespace nabi::Utils::DirectXUtils;

		dx::XMFLOAT3 const radiusVector = { radius, radius, radius };
		aabb.m_MinExtents = Float3Subtract(aabb.m_MinExtents, radiusVector);
		aabb.m_MaxExtents = Float3Add(aabb.m_MaxExtents, radiusVector);
	}

	dx::XMFLOAT3 CalculateCollisionNormal(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT
	{
		using namespace nabi::Utils::DirectXUtils;

		auto calculateNormalHelper =
			[](float const lhsAxisMin, float const lhsAxisMax, float const rhsAxisMin, float const rhsAxisMax) -> float
			{
				return ((rhsAxisMin + rhsAxisMax) * 0.5f) - ((lhsAxisMin + lhsAxisMax) * 0.5f);
			};

		dx::XMFLOAT3 normal = c_Float3Zero;
		
		normal.x = calculateNormalHelper(lhs.m_MinExtents.x, lhs.m_MaxExtents.x, rhs.m_MinExtents.x, rhs.m_MaxExtents.x);
		normal.y = calculateNormalHelper(lhs.m_MinExtents.y, lhs.m_MaxExtents.y, rhs.m_MinExtents.y, rhs.m_MaxExtents.y);
		normal.z = calculateNormalHelper(lhs.m_MinExtents.z, lhs.m_MaxExtents.z, rhs.m_MinExtents.z, rhs.m_MaxExtents.z);

		return normal;
	}

	dx::XMFLOAT3 CalculatePenetrationDepth(AABB const& lhs, AABB const& rhs) NABI_NOEXCEPT
	{
		using namespace nabi::Utils::DirectXUtils;

		auto calculatePentrationHelper =
			[](float const lhsAxisMin, float const lhsAxisMax, float const rhsAxisMin, float const rhsAxisMax) -> float
		{
			return std::min(lhsAxisMax, rhsAxisMax) - std::max(lhsAxisMin, rhsAxisMin);
		};

		dx::XMFLOAT3 depth = c_Float3Zero;

		depth.x = calculatePentrationHelper(lhs.m_MinExtents.x, lhs.m_MaxExtents.x, rhs.m_MinExtents.x, rhs.m_MaxExtents.x);
		depth.y = calculatePentrationHelper(lhs.m_MinExtents.y, lhs.m_MaxExtents.y, rhs.m_MinExtents.y, rhs.m_MaxExtents.y);
		depth.z = calculatePentrationHelper(lhs.m_MinExtents.z, lhs.m_MaxExtents.z, rhs.m_MinExtents.z, rhs.m_MaxExtents.z);

		return depth;
	}

	void CalculateSmallestPentrationDepth(dx::XMFLOAT3 const& penetrationDepth, float& penetration, dx::XMFLOAT3& normal) NABI_NOEXCEPT
	{
		if (penetrationDepth.x < penetrationDepth.y && penetrationDepth.x < penetrationDepth.z) 
		{
			penetration = penetrationDepth.x;
			normal.x = normal.x < 0.0f ? -1.0f : 1.0f;
		}
		else if (penetrationDepth.y < penetrationDepth.z)
		{
			penetration = penetrationDepth.y;
			normal.y = normal.y < 0.0f ? -1.0f : 1.0f;
		}
		else 
		{
			penetration = penetrationDepth.z;
			normal.z = normal.z < 0.0f ? -1.0f : 1.0f;
		}
	}

	std::string AABBToString(AABB const& aabb, std::optional<std::string> const aabbName) NABI_NOEXCEPT
	{
		std::string const maxExtents = 
			std::to_string(aabb.m_MaxExtents.x) + " " + 
			std::to_string(aabb.m_MaxExtents.y) + " " + 
			std::to_string(aabb.m_MaxExtents.z);

		std::string const minExtents =
			std::to_string(aabb.m_MinExtents.x) + " " +
			std::to_string(aabb.m_MinExtents.y) + " " +
			std::to_string(aabb.m_MinExtents.z);

		std::string const name = aabbName.has_value() ? aabbName.value() : "";

		return name + " Max: " + maxExtents + " | Min: " + minExtents;
	}
} // namespace nabi::Physics::CollisionSolvers
