#pragma once
#ifndef STARDUST_PARTICLE_SYSTEM_H
#define STARDUST_PARTICLE_SYSTEM_H

#include "stardust/ecs/components/SpriteComponent.h"
#include "stardust/ecs/components/TransformComponent.h"
#include "stardust/particles/Particle.h"
#include "stardust/particles/ParticleData.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class ParticleSystem final
    {
        static constexpr usize s_MaxParticleCount = 4'000u;

        List<Particle> m_particlePool;
        usize m_particlePoolIndex = 0u;
        HashMap<usize, ObserverPointer<Particle>> m_activeParticles{ };

        f32 m_gravity = 0.0f;
        f32 m_wind = 0.0f;

    public:
        [[nodiscard]] static constexpr auto MaxParticleCount() noexcept -> usize { return s_MaxParticleCount; }

        ParticleSystem();

        auto Update(const f32 deltaTime) -> void;

        [[nodiscard]] auto GenerateParticleComponents() const -> Generator<const Pair<components::Transform, components::Sprite>>;

        auto Emit(const ParticleData& particleData) -> void;
        auto KillAllParticles() -> void;

        auto RepositionAllActiveParticles(const Vector2 relativePosition) -> void;
        auto ResizeAllActiveParticles(const f32 relativeScale) -> void;

        [[nodiscard]] inline auto GetActiveParticleCount() const noexcept -> usize { return m_activeParticles.size(); }

        [[nodiscard]] inline auto GetGravity() const noexcept -> f32 { return m_gravity; }
        [[nodiscard]] inline auto SetGravity(const f32 gravity) noexcept -> void { m_gravity = gravity; }

        [[nodiscard]] inline auto GetWind() const noexcept -> f32 { return m_wind; }
        [[nodiscard]] inline auto SetWind(const f32 wind) noexcept -> void { m_wind = wind; }
    };
}


#endif
