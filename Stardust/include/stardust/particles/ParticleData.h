#pragma once
#ifndef STARDUST_PARTICLE_DATA_H
#define STARDUST_PARTICLE_DATA_H

#include "stardust/animation/Easings.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/Colour.h"
#include "stardust/graphics/Colours.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/math/Math.h"

namespace stardust
{
    struct ParticleData
    {
        Vec2 initialPosition = Vec2Zero;
        f32 initialRotation = 0.0f;

        Vec2 minVelocity = Vec2Zero;
        Vec2 maxVelocity = Vec2Zero;
        f32 acceleration = 1.0f;

        f32 minAngularVelocity = 0.0f;
        f32 maxAngularVelocity = 0.0f;
        f32 angularAcceleration = 1.0f;

        bool isAffectedByGravity = false;
        bool isAffectedByWind = false;

        Vec2 minSize = Vec2One;
        Vec2 maxSize = Vec2One;
        f32 sizeUpdateMultipler = 1.0f;
        bool keepAsSquare = false;
        bool shiftToCentre = false;

        Colour startColour = colours::White;
        Colour endColour = colours::White;
        ObserverPtr<const Texture> texture = nullptr;
        Optional<TextureCoordinatePair> textureArea = NullOpt;

        EasingFunction colourEasingFunction = easings::EaseLinear;

        f32 minLifetime = 0.0f;
        f32 maxLifetime = 0.0f;
    };
}

#endif