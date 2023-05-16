#pragma once

// A collection of core includes to make life easier

// --- Standard Lib ---
#include <bitset>
#include <cstdint>
#include <stack>
#include <string>

// --- Windows / DirectX ---
#include "WinCore.h" // TODO (have to double check if this should go before or after standard)
#include "DirectXCore.h"

#pragma comment(lib, "XInput.lib") 
#include <Xinput.h>

// --- Nabi ---
// Entt
#include "entt.h"
using namespace entt::literals;

// Nabi Core
#include "Context.h"
#include "Defines.h"
#include "GameTime.h"

// ECS
#include "ComponentBase.h"
#include "SystemBase.h"

// Reflection
#include "ReflectionGlobals.h"
#include "ReflectionMacros.h"

#include "EnumConverter.h"
#include "StringConverter.h"

// Testing
#include "gtest/gtest.h" // Note - This has been done https://stackoverflow.com/questions/68193449/disable-warnings-from-google-test
#include "BuildUtils.h"
#include "TestUtils.h"

// Utils
#include "DebugUtils.h" // (includes Logger)
#include "MathUtils.h"
#include "StringUtils.h"
#include "TypeUtils.h"
