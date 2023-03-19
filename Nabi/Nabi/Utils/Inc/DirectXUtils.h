#pragma once
#include "DirectXCore.h"

#include <string>

#include "TypeUtils.h"

namespace nabi::Utils::DirectXUtils
{
	[[nodiscard]] std::string MatrixToString(dx::XMMATRIX const& matrix) NABI_NOEXCEPT;

	// am i being dumb or do these not exist in dx?? probs the former...
	[[nodiscard]] inline dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x + scalar, float2.y + scalar); };
	[[nodiscard]] inline dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x + float2Rhs.x, float2Lhs.y + float2Rhs.y); };
	[[nodiscard]] inline dx::XMFLOAT2 Float2Add(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x + float3Rhs.x, float2Lhs.y + float3Rhs.y); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x + scalar, float3.y + scalar, float3.z + scalar); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const& float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x + float3Rhs.x, float3Lhs.y + float3Rhs.y, float3Lhs.z + float3Rhs.z); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Add(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT2 const& float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x + float2Rhs.x, float3Lhs.y + float2Rhs.y, float3Lhs.z + 0.0f); };

	[[nodiscard]] inline dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x - scalar, float2.y - scalar); };
	[[nodiscard]] inline dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x - float2Rhs.x, float2Lhs.y - float2Rhs.y); };
	[[nodiscard]] inline dx::XMFLOAT2 Float2Subtract(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x - float3Rhs.x, float2Lhs.y - float3Rhs.y); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x - scalar, float3.y - scalar, float3.z - scalar); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x - float3Rhs.x, float3Lhs.y - float3Rhs.y, float3Lhs.z - float3Rhs.z); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Subtract(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x - float2Rhs.x, float3Lhs.y - float2Rhs.y, float3Lhs.z - 0.0f ); };

	[[nodiscard]] inline dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x * scalar, float2.y * scalar); };
	[[nodiscard]] inline dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x * float2Rhs.x, float2Lhs.y * float2Rhs.y); };
	[[nodiscard]] inline dx::XMFLOAT2 Float2Multiply(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x * float3Rhs.x, float2Lhs.y * float3Rhs.y); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x * scalar, float3.y * scalar, float3.z * scalar); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x * float3Rhs.x, float3Lhs.y * float3Rhs.y, float3Lhs.z * float3Rhs.z); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Multiply(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x * float2Rhs.x, float3Lhs.y * float2Rhs.y, float3Lhs.z * 1.0f); };

	[[nodiscard]] inline dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const float2, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2.x / scalar, float2.y / scalar); };
	[[nodiscard]] inline dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x / float2Rhs.x, float2Lhs.y / float2Rhs.y); };
	[[nodiscard]] inline dx::XMFLOAT2 Float2Divide(dx::XMFLOAT2 const float2Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT2(float2Lhs.x / float3Rhs.x, float2Lhs.y / float3Rhs.y); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const float3, float const scalar) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3.x / scalar, float3.y / scalar, float3.z / scalar); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT3 const float3Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x / float3Rhs.x, float3Lhs.y / float3Rhs.y, float3Lhs.z / float3Rhs.z); };
	[[nodiscard]] inline dx::XMFLOAT3 Float3Divide(dx::XMFLOAT3 const float3Lhs, dx::XMFLOAT2 const float2Rhs) NABI_NOEXCEPT
	{ return dx::XMFLOAT3(float3Lhs.x / float2Rhs.x, float3Lhs.y / float2Rhs.y, float3Lhs.z / 1.0f ); };
} // namespace nabi::Utils::DirectXUtils
