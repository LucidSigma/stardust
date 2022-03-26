#pragma once
#ifndef STARDUST_PARTICLE_H
#define STARDUST_PARTICLE_H

#include <functional>

#include "stardust/animation/easings/Easings.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Graphics.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    enum class ParticleCallbackResult
    {
        KeepAlive,
        Kill,
    };

    using ParticleCallback = std::function<auto(struct Particle&, Optional<Any>&) -> ParticleCallbackResult>;

    struct Particle final
    {
        Vector2 position = Vector2Zero;
        f32 rotation = 0.0f;

        Vector2 velocity = Vector2Zero;
        f32 acceleration = 0.0f;

        f32 angularVelocity = 0.0f;
        f32 angularAcceleration = 0.0f;
        Optional<Vector2> pivot = None;

        bool isAffectedByGravity = false;
        bool isAffectedByWind = false;

        Vector2 size = Vector2One;
        f32 sizeUpdateMultiplier = 1.0f;

        Optional<Vector2> shear = None;

        Colour currentColour = colours::White;
        Colour startColour = colours::White;
        Colour endColour = colours::White;

        ObserverPointer<const graphics::Texture> texture = nullptr;
        Optional<graphics::TextureCoordinatePair> textureArea = None;
        graphics::Reflection reflection = graphics::Reflection::None;

        EasingFunction colourEasingFunction = easings::EaseLinear;

        f32 totalLifetime = 0.0f;
        f32 lifetimeRemaining = 0.0f;

        Optional<ParticleCallback> callback = None;
        Optional<Any> callbackUserData = None;

        bool isActive = false;
    };
}

#endif
