#pragma once
#include "Core.h"

#ifdef RUN_TESTS

namespace nabitest::ECS
{
	class MockSystem final : public nabi::ECS::SystemBase
	{
	public:
		MockSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~MockSystem();

		void Update(/*game time*/) const;
		void Render(/*game time*/) const;

		inline int GetMockSystemData() const { return m_MockSystemData; }

	private:
		int m_MockSystemData;

		REFLECT_PRIVATES(MockSystem)
	};
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
