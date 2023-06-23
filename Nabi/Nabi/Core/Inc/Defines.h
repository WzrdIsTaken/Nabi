#pragma once

// A little clunky, but these guys can be used to toggle on / off different engine features

// Core defines
#define USE_EVENT_SYSTEM_UPDATE
#define USE_CORE_FUNCTIONALITY_MULTITHREADING
//#define USE_NABI_NOEXCEPT

// Debug defines
#ifdef _DEBUG
	#define RUN_TESTS		 // gMock Tests
	//#define RUN_SLOW_TESTS   // Any tests which take over ~10ms to run. Make sure to run before commiting
	#define USE_DEBUG_UTILS  // Asserts + Logging + Sneaky hacks
#else
	// Can shift around the defines if need to test stuff
#endif // ifdef _DEBUG

// Value defines
#define NABI_SUCCESS 0
#define NABI_FAIL   -1

// Development defines
#define ENGINE_DEVELOPMENT

// Defines based off other defines!
#ifndef USE_NABI_NOEXCEPT
	#define NABI_NOEXCEPT 
#else
	#define NABI_NOEXCEPT noexcept
#endif // ifndef USE_NABI_NOEXCEPT
