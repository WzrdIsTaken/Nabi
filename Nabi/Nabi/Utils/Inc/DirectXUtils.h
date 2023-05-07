#pragma once
#include "DirectXCore.h"

#include <string>

#include "TypeUtils.h"

#define SAFE_RELEASE(obj) if ( (obj) != NULL    ) { (obj)->Release(); (obj) = NULL;    }
#define SAFE_DELETE(obj)  if ( (obj) != nullptr ) { delete (obj);     (obj) = nullptr; }

namespace nabi::Utils::DirectXUtils
{
	[[nodiscard]] std::string MatrixToString(dx::XMMATRIX const& matrix) NABI_NOEXCEPT;

	// am i being dumb or do these not exist in dx?? probs the former...
	[[nodiscard]] inline bool Float2Equal(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMVector2Equal(dx::XMLoadFloat2(&float2Lhs), dx::XMLoadFloat2(&float2Rhs)); }
	[[nodiscard]] inline bool Float3Equal(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMVector2Equal(dx::XMLoadFloat3(&float3Lhs), dx::XMLoadFloat3(&float3Rhs)); }
	[[nodiscard]] inline bool Int2Equal(dx::XMINT2 const int2Lhs, dx::XMINT2 const& int2Rhs) NABI_NOEXCEPT
	{ return int2Lhs.x == int2Rhs.x && int2Lhs.y == int2Rhs.y; }
	[[nodiscard]] inline bool Int3Equal(dx::XMINT3 const int3Lhs, dx::XMINT3 const& int3Rhs) NABI_NOEXCEPT
	{ return int3Lhs.x == int3Rhs.x && int3Lhs.y == int3Rhs.y && int3Lhs.z == int3Rhs.z; }

	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x + scalar, float2.y + scalar); };
	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x + float2Rhs.x, float2Lhs.y + float2Rhs.y); };
	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x + float3Rhs.x, float2Lhs.y + float3Rhs.y); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x + scalar, float3.y + scalar, float3.z + scalar); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x + float3Rhs.x, float3Lhs.y + float3Rhs.y, float3Lhs.z + float3Rhs.z); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x + float2Rhs.x, float3Lhs.y + float2Rhs.y, float3Lhs.z + 0.0f); };

	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x - scalar, float2.y - scalar); };
	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x - float2Rhs.x, float2Lhs.y - float2Rhs.y); };
	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x - float3Rhs.x, float2Lhs.y - float3Rhs.y); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x - scalar, float3.y - scalar, float3.z - scalar); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x - float3Rhs.x, float3Lhs.y - float3Rhs.y, float3Lhs.z - float3Rhs.z); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x - float2Rhs.x, float3Lhs.y - float2Rhs.y, float3Lhs.z - 0.0f ); };

	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x * scalar, float2.y * scalar); };
	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x * float2Rhs.x, float2Lhs.y * float2Rhs.y); };
	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x * float3Rhs.x, float2Lhs.y * float3Rhs.y); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x * scalar, float3.y * scalar, float3.z * scalar); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x * float3Rhs.x, float3Lhs.y * float3Rhs.y, float3Lhs.z * float3Rhs.z); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x * float2Rhs.x, float3Lhs.y * float2Rhs.y, float3Lhs.z * 1.0f); };

	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x / scalar, float2.y / scalar); };
	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x / float2Rhs.x, float2Lhs.y / float2Rhs.y); };
	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x / float3Rhs.x, float2Lhs.y / float3Rhs.y); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x / scalar, float3.y / scalar, float3.z / scalar); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x / float3Rhs.x, float3Lhs.y / float3Rhs.y, float3Lhs.z / float3Rhs.z); };
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x / float2Rhs.x, float3Lhs.y / float2Rhs.y, float3Lhs.z / 1.0f ); };

	[[nodiscard]] constexpr inline dx::XMFLOAT2 Float2Squared(dx::XMFLOAT2 const float2) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x * float2.x, float2.y * float2.y); }
	[[nodiscard]] constexpr inline dx::XMFLOAT3 Float3Squared(dx::XMFLOAT3 const float3) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x * float3.x, float3.y * float3.y, float3.z * float3.z); }
} // namespace nabi::Utils::DirectXUtils
