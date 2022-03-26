#pragma once
#ifndef STARDUST_SORTING_LAYER_H
#define STARDUST_SORTING_LAYER_H

#include <compare>

#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        class SortingLayer final
        {
        private:
            f32 m_z = 0.0f;

        public:
            explicit SortingLayer(const f32 z = 0.0f);

            [[nodiscard]] auto operator ==(const SortingLayer&) const noexcept -> bool = default;
            [[nodiscard]] auto operator !=(const SortingLayer&) const noexcept -> bool = default;

            [[nodiscard]] inline auto operator <(const SortingLayer& other) const noexcept -> bool { return m_z < other.m_z; }
            [[nodiscard]] inline auto operator <=(const SortingLayer& other) const noexcept -> bool { return m_z <= other.m_z; }
            [[nodiscard]] inline auto operator >(const SortingLayer& other) const noexcept -> bool { return m_z > other.m_z; }
            [[nodiscard]] inline auto operator >=(const SortingLayer& other) const noexcept -> bool { return m_z >= other.m_z; }

            [[nodiscard]] inline auto operator <=>(const SortingLayer& other) const noexcept -> std::partial_ordering { return m_z <=> other.m_z; }

            [[nodiscard]] inline auto GetZ() const noexcept -> f32 { return m_z; }
            [[nodiscard]] inline auto SetZ(const f32 z) noexcept -> void { m_z = z; }
        };
    }
}

#endif
