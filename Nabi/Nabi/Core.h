#pragma once

// A collection of core includes to make life easier

// DirectX
// #include DirectXCore

// Entt
#include "Libraries/Inc/entt/entt.h"
using namespace entt::literals;

// Nabi Defines
#include "Core/Inc/Defines.h"

// ECS
#include "ECS/Inc/ComponentBase.h"
#include "ECS/Inc/SystemBase.h"

// Reflection
#include "Reflection/Inc/ReflectionGlobals.h"
#include "Reflection/Inc/ReflectionMacros.h"

#include "Reflection/Inc/EnumConverter.h"
#include "Reflection/Inc/StringConverter.h"

// Testing
#include "gtest/gtest.h" // Note - This has been done https://stackoverflow.com/questions/68193449/disable-warnings-from-google-test
#include "Utils/Inc/BuildUtils.h"
#include "Utils/Inc/TestUtils.h"

// Debug (includes Logger)
#include "Utils/Inc/DebugUtils.h"

// Maths
#include "Utils/Inc/MathUtils.h"

// Types
#include "Utils/Inc/TypeUtils.h"

// Standard Lib
#include <string>
/* more in the future i'm sure... */
