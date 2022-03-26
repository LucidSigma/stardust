#pragma once
#ifndef STARDUST_PARTICLE_DATA_H
#define STARDUST_PARTICLE_DATA_H

#include "stardust/animation/easings/Easings.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Graphics.h"
#include "stardust/math/Math.h"
#include "stardust/particles/Particle.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    struct ParticleData final
    {
        Vector2 initialPosition = Vector2Zero;
        f32 initialRotation = 0.0f;

        Pair<Vector2, Vector2> initialVelocityRange = { Vector2Zero, Vector2Zero };
        Pair<f32, f32> initialAccelerationRange = { 0.0f, 0.0f };

        Pair<f32, f32> initialAngularVelocityRange = { 0.0f, 0.0f};
        Pair<f32, f32> initialAngularAccelerationRange = { 0.0f, 0.0f };
        Optional<Vector2> pivot = None;

        bool isAffectedByGravity = false;
        bool isAffectedByWind = false;

        Pair<Vector2, Vector2> initialSizeRange = { Vector2One, Vector2One };
        f32 sizeUpdateMultiplier = 1.0f;
        bool keepAsSquare = false;

        Optional<Pair<Vector2, Vector2>> initialShearRange = None;

        Colour startColour = colours::White;
        Colour endColour = colours::White;

        ObserverPointer<const graphics::Texture> texture = nullptr;
        Optional<graphics::TextureCoordinatePair> textureArea = None;
        graphics::Reflection reflection = graphics::Reflection::None;

        EasingFunction colourEasingFunction = easings::EaseLinear;

        Pair<f32, f32> initialLifetimeRange = { 0.0f, 0.0f };

        Optional<ParticleCallback> callback = None;
        Optional<Any> callbackUserData = None;
    };
}

#endif
