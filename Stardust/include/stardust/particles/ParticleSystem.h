#pragma once
#ifndef STARDUST_PARTICLE_SYSTEM_H
#define STARDUST_PARTICLE_SYSTEM_H

#include "stardust/animation/Easings.h"
#include "stardust/camera/Camera2D.h"
#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/Colour.h"
#include "stardust/graphics/Colours.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/math/Math.h"
#include "stardust/particles/ParticleData.h"

namespace stardust
{
    class ParticleSystem
    {
    private:
        struct Particle
        {
            Vec2 position = Vec2Zero;
            f32 rotation = 0.0f;

            Vec2 velocity = Vec2Zero;
            f32 acceleration = 1.0f;

            f32 angularVelocity = 0.0f;
            f32 angularAcceleration = 1.0f;

            bool isAffectedByGravity = false;
            bool isAffectedByWind = false;

            Vec2 size = Vec2One;
            f32 sizeUpdateMultipler = 1.0f;

            Colour currentColour = colours::White;
            Colour startColour = colours::White;
            Colour endColour = colours::White;
            ObserverPtr<const Texture> texture = nullptr;
            Optional<TextureCoordinatePair> textureArea = NullOpt;

            EasingFunction colourEasingFunction = easings::EaseLinear;

            f32 lifetime = 0.0f;
            f32 lifetimeRemaining = 0.0f;

            bool isActive = false;
        };

        static constexpr usize s_ParticleCount = 1'000u;

        Vector<Particle> m_particlePool;
        usize m_particlePoolIndex = s_ParticleCount - 1u;
        HashMap<usize, ObserverPtr<Particle>> m_activeParticles{ };

        f32 m_gravity = 0.0f;
        f32 m_wind = 0.0f;

    public:
        [[nodiscard]] static constexpr usize GetMaxParticleCount() noexcept { return s_ParticleCount; }

        ParticleSystem();
        ~ParticleSystem() noexcept = default;

        void Update(const f32 deltaTime);
        void RenderInWorld(Renderer& renderer, const Camera2D& camera) const;
        void RenderOnScreen(Renderer& renderer) const;

        void Emit(const ParticleData& particleData);
        void KillAllParticles();

        void RepositionAllActiveParticles(const Vec2& relativePosition);
        void ResizeAllActiveParticles(const f32 relativeScale);

        [[nodiscard]] inline usize GetActiveParticleCount() const noexcept { return m_activeParticles.size(); }

        [[nodiscard]] inline f32 GetGravity() const noexcept { return m_gravity; }
        [[nodiscard]] inline void SetGravity(const f32 gravity) noexcept { m_gravity = gravity; }

        [[nodiscard]] inline f32 GetWind() const noexcept { return m_wind; }
        [[nodiscard]] inline void SetWind(const f32 wind) noexcept { m_wind = wind; }
    };
}

#endif