#pragma once
#ifndef STARDUST_ENTITY_HANDLE_H
#define STARDUST_ENTITY_HANDLE_H

#include <entt/entt.hpp>

namespace stardust
{
    using EntityHandle = entt::entity;

    constexpr EntityHandle NullEntityHandle = entt::null;
}

#endif
