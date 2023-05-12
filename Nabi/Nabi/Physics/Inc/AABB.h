#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

#include "DirectXUtils.h"

namespace nabi::Physics
{
	struct AABB final
	{
		dx::XMFLOAT3 m_MinExtents;
		dx::XMFLOAT3 m_MaxExtents;
	};
} // namespace nabi::Physics
