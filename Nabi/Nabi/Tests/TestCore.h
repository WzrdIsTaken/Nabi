#pragma once

// Contains a bunch of core test includes to make life easier

// Nabi Core
#include "../Core.h"

// Mock Objects
#include "Inc/MockObjects/MockComponent.h"
#include "Inc/MockObjects/MockCustomDataType.h"
#include "Inc/MockObjects/MockEnum.h"
#include "Inc/MockObjects/MockGameCore.h"
#include "Inc/MockObjects/MockSystem.h"

// Helpers
#include "Inc/Helpers/Comparison.h"

// Using - I thought this might be good so in the tests every test thing exists at the same level + its easy to differentiate between mock ecs stuff and 'real' ecs stuff 
#ifdef RUN_TESTS
	using namespace nabitest::ECS;
#endif // ifdef RUN_TESTS
