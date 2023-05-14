#include "DirectXUtils.h"

#include <sstream>

namespace nabi::Utils::DirectXUtils
{
	std::string MatrixToString(dx::XMMATRIX const& matrix) NABI_NOEXCEPT
	{
		int constexpr rowLength    = 4;
		int constexpr collumLength = 4;

		dx::XMFLOAT4X4 matrixAsFloat4;
		dx::XMStoreFloat4x4(&matrixAsFloat4, matrix);

		std::ostringstream matrixStr;

		for (int row = 0; row < rowLength; ++row)
		{
			for (int collum = 0; collum < collumLength; ++collum)
			{
				matrixStr << matrixAsFloat4.m[row][collum];
				matrixStr << ", ";
			}
		}

		return matrixStr.str();
	}

	std::string Float3ToString(dx::XMFLOAT3 const& float3) NABI_NOEXCEPT
	{
		std::string const x = "x: " + std::to_string(float3.x);
		std::string const y = "y: " + std::to_string(float3.y);
		std::string const z = "z: " + std::to_string(float3.z);

		return x + " " + y + " " + z;
	}
} // namespace nabi::Utils::DirectXUtils
