#pragma once

// A collection of core includes to make life easier

// DirectX
// #include DirectXCore

// Entt
#include "Libraries/entt/entt.h"
using namespace entt::literals;

// Nabi Defines
#include "Core/Defines.h"

// ECS
#include "ECS/ComponentBase.h"
#include "ECS/SystemBase.h"

// Reflection
#include "Reflection/ReflectionGlobals.h"
#include "Reflection/ReflectionMacros.h"

#include "Reflection/EnumConverter.h"
#include "Reflection/StringConverter.h"

// Testing
#include "gtest/gtest.h" // Note - This has been done https://stackoverflow.com/questions/68193449/disable-warnings-from-google-test
#include "Utils/TestUtils.h"

// Debug (includes Logger)
#include "Utils/DebugUtils.h"
