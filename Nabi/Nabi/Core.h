#pragma once

// A collection of core includes to make life easier

// --- Standard Lib ---
#include <any>
#include <bitset>
#include <cstdint>
#include <stack>
#include <string>

// --- Windows / DirectX ---
#include "WinCore.h"
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

// Utils
#include "BuildUtils.h"
#include "DebugUtils.h" // (includes Logger)
#include "MathUtils.h"
#include "StringUtils.h"
#include "TestUtils.h"
#include "TypeUtils.h"
