#pragma once

// Contains a bunch of core test includes to make life easier

// Nabi Core
#include "Core.h"

// gTest
#include "gtest/gtest.h" // Note - This has been done https://stackoverflow.com/questions/68193449/disable-warnings-from-google-test

// Mock Objects
#include "MockObjects/MockComponent.h"
#include "MockObjects/MockCustomDataType.h"
#include "MockObjects/MockEnum.h"
#include "MockObjects/MockGameCore.h"
#include "MockObjects/MockSystem.h"

// Helpers
#include "Helpers/Comparison.h"

#ifdef RUN_TESTS
	// Using - I thought this might be good so in the tests every test thing exists at the same level + its easy to differentiate between mock ecs stuff and 'real' ecs stuff 
	using namespace nabitest::ECS;

	#define LOG_NOT_RUNNING_SLOW_TEST_WARNING \
		LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_TEST, "Not running slow test " << WRAP(__FUNCTION__, "'") << " - make sure this is done before committing", LOG_END);
#endif // ifdef RUN_TESTS
