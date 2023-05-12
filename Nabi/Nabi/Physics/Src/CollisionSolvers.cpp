#include "EngineCore.h"

#include "CollisionSolvers.h"

namespace nabi::Physics::CollisionSolvers
{
} // namespace nabi::Physics::CollisionSolvers

namespace nabi::Physics::AABBSolvers
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
} // namespace nabi::Physics::AABBSolvers
