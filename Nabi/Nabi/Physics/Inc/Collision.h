#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

namespace nabi::Physics
{
	struct Collision final
	{
		dx::XMFLOAT3 m_Normal;
		float m_Depth;
	};
} // namespace nabi::Physics
