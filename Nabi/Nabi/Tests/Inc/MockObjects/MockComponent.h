#pragma once
#include "Core.h"

#include "ComponentBase.h"
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

	struct MockComponentWithDirectXTypes final : public nabi::ECS::ComponentBase
	{
		MockComponentWithDirectXTypes()
			: m_Float2(0.0f, 1.0f)
			, m_Float3(2.0f, 3.0f, 4.0f)
			, m_Int2(5, 6)
			, m_Int3(7, 8, 9)
		{
		}

		dx::XMFLOAT2 m_Float2;
		dx::XMFLOAT3 m_Float3;
		dx::XMINT2 m_Int2;
		dx::XMINT3 m_Int3;
	};

	struct MockComponentWithContainers final : public nabi::ECS::ComponentBase
	{
		MockComponentWithContainers()
			: m_IntVector{1, 2, 3}
			, m_CustomTypeVector{{0.1, true}, {0.2, false}}
		{
		}

		std::vector<int> m_IntVector;
		std::vector<MockCustomDataType> m_CustomTypeVector;
	};
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
