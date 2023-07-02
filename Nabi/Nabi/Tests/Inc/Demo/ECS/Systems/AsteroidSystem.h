#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

namespace ecs
{
	class AsteroidSystem final : public nabi::ECS::SystemBase
	{
	public:
		AsteroidSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~AsteroidSystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		void MoveAndRotateAsteroids(float const dt) const;
		void CheckInput() const;

		static entt::hashed_string constexpr c_AsteroidGroupName = "DemoEntities"_hs;

		REFLECT_PRIVATES(AsteroidSystem)
	};
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
