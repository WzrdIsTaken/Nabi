#pragma once
#include "DirectXCore.h"

namespace nabi::Rendering
{
	struct Vertex
	{
		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT3 m_UV;
		dx::XMFLOAT3 m_Normal;
	};
} // namespace nabi::Rendering
