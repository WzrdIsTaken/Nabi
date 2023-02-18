#pragma once

// A collection of core includes to make life easier

// --- Standard Lib ---
#include <string>
/* more in the future i'm sure... 
   is it dumb to have like a standard.h file with all the standard includes i use? can include here and in enginecore*/

// --- Windows / DirectX ---
#include "WinCore.h" // TODO (have to double check if this should go before or after standard)
#include "DirectXCore.h"

// --- Nabi ---
// Entt
#include "entt.h"
using namespace entt::literals;

// Nabi Defines
#include "Defines.h"

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
#include "TypeUtils.h"
