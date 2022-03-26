#pragma once
#ifndef STARDUST_EASINGS_H
#define STARDUST_EASINGS_H

#include <functional>

#include "stardust/types/Primitives.h"

namespace stardust
{
    using EasingFunction = std::function<auto(f32) -> f32>;

    namespace easings
    {
        [[nodiscard]] extern auto EaseIn(const f32 value, const f32 magnitude) -> f32;
        [[nodiscard]] extern auto EaseOut(const f32 value, const f32 magnitude) -> f32;
        [[nodiscard]] extern auto EaseInOut(const f32 value, const f32 magnitude) -> f32;

        [[nodiscard]] extern auto EaseLinear(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInQuad(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutQuad(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutQuad(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInCubic(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutCubic(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutCubic(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInQuart(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutQuart(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutQuart(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInQuint(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutQuint(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutQuint(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInSine(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutSine(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutSine(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInExponential(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutExponential(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutExponential(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInCircle(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutCircle(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutCircle(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInBack(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutBack(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutBack(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInElastic(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutElastic(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutElastic(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseInBounce(const f32 value) -> f32;
        [[nodiscard]] extern auto EaseOutBounce(f32 value) -> f32;
        [[nodiscard]] extern auto EaseInOutBounce(const f32 value) -> f32;

        [[nodiscard]] extern auto EaseHeavisideStep(const f32 value) -> f32;
    }
}

#endif
