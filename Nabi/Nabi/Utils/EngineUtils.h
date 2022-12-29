#pragma once

#include "../Core/Defines.h"

#ifndef USE_DEBUG_UTILS
	#define NABI_NOEXCEPT noexcept
#else
	#define NABI_NOEXCEPT
#endif // ifndef USE_DEBUG_UTLS
