#pragma once

#include "Defines.h"

#ifndef USE_DEBUG_UTILS
    #define NABI_NOEXCEPT noexcept
#else
    #define NABI_NOEXCEPT
#endif // ifndef USE_DEBUG_UTLS

#define DELETE_COPY_MOVE_CONSTRUCTORS(type) \
    DELETE_COPY_CONSTRUCTORS(type) \
    DELETE_MOVE_CONSTRUCTORS(type)

#define DELETE_COPY_CONSTRUCTORS(type) \
     type(type&) = delete; \
     type(type const&) = delete; \

#define DELETE_MOVE_CONSTRUCTORS(type) \
    type(type&&) = delete; \
    type(type const&&) = delete;

//#define interface __interface

// ngl i don't think 'TypeUtils' is the best name... if I think of something better I'll change it!
// types are classes and structs.. so that was my reasoning. but 'ClassStructUtils' seemed even worse xD
// but now i've started using this in namespaces as well. gj ben good name
