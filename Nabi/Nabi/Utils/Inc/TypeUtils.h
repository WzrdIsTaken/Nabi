#pragma once

#include "Defines.h"

#ifndef USE_DEBUG_UTILS
    #define NABI_NOEXCEPT noexcept
#else
    #define NABI_NOEXCEPT
#endif // ifndef USE_DEBUG_UTLS

#define DELETE_COPY_MOVE_CONSTRUCTORS(type) \
    type(type&) = delete; \
    type(type&&) = delete; \
    type(type const&) = delete; \
    type(type const&&) = delete;

/*
* Is this a good idea? Probs not..
* #define DEFAULT_CTOR_DTOR(type) \
*    type() NABI_NOEXCEPT = default; \
*    ~type() = default;
* #define DEFAULT_CTOR_DTOR_VIRTUAL(type) \
*     type() NABI_NOEXCEPT = default; \
*     virtual ~type() = default;
* #define DEFAULT_CTOR_DTOR_OVERRIDE(type) \
*     type() NABI_NOEXCEPT = default; \
*     ~type() override = default;
*/

// ngl i don't think 'TypeUtils' is the best name... if I think of something better I'll change it!
// types are classes and structs.. so that was my reasoning. but 'ClassStructUtils' seemed even worse xD
// but now i've started using this in namespaces as well. gj ben good name
