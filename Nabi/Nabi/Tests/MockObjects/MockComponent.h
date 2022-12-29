#pragma once

#include "../../Core.h"

#include <string>

#include "../../ECS/ComponentBase.h"
#include "MockCustomDataType.h"
#include "MockEnum.h"

#ifdef RUN_TESTS

namespace nabitest::ECS
{
	struct MockComponent final : public nabi::ECS::ComponentBase
	{
		MockComponent()
			: m_IntType(5)
			, m_FloatType(20.6f)
			, m_StringType("I wrote this on 20 / 11 / 2022")
			, m_CustomType({ 6.4, true })
			, m_Enum(MockEnum::Ben)
		{
		}

		int m_IntType;
		float m_FloatType;
		std::string m_StringType;
		MockCustomDataType m_CustomType;
		MockEnum m_Enum;
	};
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
