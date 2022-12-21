#pragma once

// Contains a bunch of core test includes to make life easier

// Nabi Core
#include "../Core.h"

// Mock Objects
#include "MockObjects/MockComponent.h"
#include "MockObjects/MockCustomDataType.h"
#include "MockObjects/MockEnum.h"
#include "MockObjects/MockGameCore.h"
#include "MockObjects/MockSystem.h"

// Helpers
#include "Helpers/Comparison.h"

// Using - I thought this might be good so in the tests every test thing exists at the same level + its easy to differentiate between mock ecs stuff and 'real' ecs stuff 
#ifdef _DEBUG
	using namespace nabitest::ECS;
#endif // ifdef _DEBUG
