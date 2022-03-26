#pragma once
#ifndef STARDUST_MATH_H
#define STARDUST_MATH_H

#include <cmath>
#include <complex>
#include <limits>
#include <numbers>
#include <ratio>
#include <type_traits>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    constexpr Vector2 Vector2Up{ 0.0f, 1.0f };
    constexpr Vector2 Vector2Down{ 0.0f, -1.0f };
    constexpr Vector2 Vector2Left{ -1.0f, 0.0f };
    constexpr Vector2 Vector2Right{ 1.0f, 0.0f };
    constexpr Vector2 Vector2Zero{ 0.0f, 0.0f };
    constexpr Vector2 Vector2One{ 1.0f, 1.0f };
    constexpr Vector2 Vector2NegOne{ -1.0f, -1.0f };
    constexpr Vector2 Vector2PosInf{ std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max() };
    constexpr Vector2 Vector2NegInf{ std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest() };

    constexpr Vector3 Vector3Up{ 0.0f, 1.0f, 0.0f };
    constexpr Vector3 Vector3Down{ 0.0f, -1.0f, 0.0f };
    constexpr Vector3 Vector3Left{ -1.0f, 0.0f, 0.0f };
    constexpr Vector3 Vector3Right{ 1.0f, 0.0f, 0.0f };
    constexpr Vector3 Vector3Forward{ 0.0f, 0.0f, 1.0f };
    constexpr Vector3 Vector3Back{ 0.0f, 0.0f, -1.0f };
    constexpr Vector3 Vector3Zero{ 0.0f, 0.0f, 0.0f };
    constexpr Vector3 Vector3One{ 1.0f, 1.0f, 1.0f };
    constexpr Vector3 Vector3NegOne{ -1.0f, -1.0f, -1.0f };
    constexpr Vector3 Vector3PosInf{ std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max() };
    constexpr Vector3 Vector3NegInf{ std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest() };

    constexpr Vector4 Vector4Zero{ 0.0f, 0.0f, 0.0f, 0.0f };
    constexpr Vector4 Vector4One{ 1.0f, 1.0f, 1.0f, 1.0f };
    constexpr Vector4 Vector4NegOne{ -1.0f, -1.0f, -1.0f, -1.0f };
    constexpr Vector4 Vector4PosInf{ std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max() };
    constexpr Vector4 Vector4NegInf{ std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest() };

    constexpr DVector2 DVector2Up{ 0.0, 1.0 };
    constexpr DVector2 DVector2Down{ 0.0, -1.0 };
    constexpr DVector2 DVector2Left{ -1.0, 0.0 };
    constexpr DVector2 DVector2Right{ 1.0, 0.0 };
    constexpr DVector2 DVector2Zero{ 0.0, 0.0 };
    constexpr DVector2 DVector2One{ 1.0, 1.0 };
    constexpr DVector2 DVector2NegOne{ -1.0, -1.0 };
    constexpr DVector2 DVector2PosInf{ std::numeric_limits<f64>::max(), std::numeric_limits<f64>::max() };
    constexpr DVector2 DVector2NegInf{ std::numeric_limits<f64>::lowest(), std::numeric_limits<f64>::lowest() };

    constexpr DVector3 DVector3Up{ 0.0, 1.0, 0.0 };
    constexpr DVector3 DVector3Down{ 0.0, -1.0, 0.0 };
    constexpr DVector3 DVector3Left{ -1.0, 0.0, 0.0 };
    constexpr DVector3 DVector3Right{ 1.0, 0.0, 0.0 };
    constexpr DVector3 DVector3Forward{ 0.0, 0.0, 1.0 };
    constexpr DVector3 DVector3Back{ 0.0, 0.0, -1.0 };
    constexpr DVector3 DVector3Zero{ 0.0, 0.0, 0.0 };
    constexpr DVector3 DVector3One{ 1.0, 1.0, 1.0 };
    constexpr DVector3 DVector3NegOne{ -1.0, -1.0, -1.0 };
    constexpr DVector3 DVector3PosInf{ std::numeric_limits<f64>::max(), std::numeric_limits<f64>::max(), std::numeric_limits<f64>::max() };
    constexpr DVector3 DVector3NegInf{ std::numeric_limits<f64>::lowest(), std::numeric_limits<f64>::lowest(), std::numeric_limits<f64>::lowest() };

    constexpr DVector4 DVector4Zero{ 0.0, 0.0, 0.0, 0.0 };
    constexpr DVector4 DVector4One{ 1.0, 1.0, 1.0, 1.0 };
    constexpr DVector4 DVector4NegOne{ -1.0, -1.0, -1.0, -1.0 };
    constexpr DVector4 DVector4PosInf{ std::numeric_limits<f64>::max(), std::numeric_limits<f64>::max(), std::numeric_limits<f64>::max(), std::numeric_limits<f64>::max() };
    constexpr DVector4 DVector4NegInf{ std::numeric_limits<f64>::lowest(), std::numeric_limits<f64>::lowest(), std::numeric_limits<f64>::lowest(), std::numeric_limits<f64>::lowest() };

    constexpr IVector2 IVector2Up{ 0, 1 };
    constexpr IVector2 IVector2Down{ 0, -1 };
    constexpr IVector2 IVector2Left{ -1, 0 };
    constexpr IVector2 IVector2Right{ 1, 0 };
    constexpr IVector2 IVector2Zero{ 0, 0 };
    constexpr IVector2 IVector2One{ 1, 1 };
    constexpr IVector2 IVector2NegOne{ -1, -1 };
    constexpr IVector2 IVector2PosInf{ std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max() };
    constexpr IVector2 IVector2NegInf{ std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min() };

    constexpr IVector3 IVector3Up{ 0, 1, 0 };
    constexpr IVector3 IVector3Down{ 0, -1, 0 };
    constexpr IVector3 IVector3Left{ -1, 0, 0 };
    constexpr IVector3 IVector3Right{ 1, 0, 0 };
    constexpr IVector3 IVector3Forward{ 0, 0, 1 };
    constexpr IVector3 IVector3Back{ 0, 0, -1 };
    constexpr IVector3 IVector3Zero{ 0, 0, 0 };
    constexpr IVector3 IVector3One{ 1, 1, 1 };
    constexpr IVector3 IVector3NegOne{ -1, -1, -1 };
    constexpr IVector3 IVector3PosInf{ std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max() };
    constexpr IVector3 IVector3NegInf{ std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min() };

    constexpr IVector4 IVector4Zero{ 0, 0, 0, 0 };
    constexpr IVector4 IVector4One{ 1, 1, 1, 1 };
    constexpr IVector4 IVector4NegOne{ -1, -1, -1, -1 };
    constexpr IVector4 IVector4PosInf{ std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max() };
    constexpr IVector4 IVector4NegInf{ std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min() };

    constexpr LVector2 LVector2Up{ 0ll, 1ll };
    constexpr LVector2 LVector2Down{ 0ll, -1ll };
    constexpr LVector2 LVector2Left{ -1ll, 0ll };
    constexpr LVector2 LVector2Right{ 1ll, 0ll };
    constexpr LVector2 LVector2Zero{ 0ll, 0ll };
    constexpr LVector2 LVector2One{ 1ll, 1ll };
    constexpr LVector2 LVector2NegOne{ -1ll, -1ll };
    constexpr LVector2 LVector2PosInf{ std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max() };
    constexpr LVector2 LVector2NegInf{ std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min() };

    constexpr LVector3 LVector3Up{ 0ll, 1ll, 0ll };
    constexpr LVector3 LVector3Down{ 0ll, -1ll, 0ll };
    constexpr LVector3 LVector3Left{ -1ll, 0ll, 0ll };
    constexpr LVector3 LVector3Right{ 1ll, 0ll, 0ll };
    constexpr LVector3 LVector3Forward{ 0ll, 0ll, 1ll };
    constexpr LVector3 LVector3Back{ 0ll, 0ll, -1ll };
    constexpr LVector3 LVector3Zero{ 0ll, 0ll, 0ll };
    constexpr LVector3 LVector3One{ 1ll, 1ll, 1ll };
    constexpr LVector3 LVector3NegOne{ -1ll, -1ll, -1ll };
    constexpr LVector3 LVector3PosInf{ std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max() };
    constexpr LVector3 LVector3NegInf{ std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min() };

    constexpr LVector4 LVectorZero{ 0ll, 0ll, 0ll, 0ll };
    constexpr LVector4 LVectorOne{ 1ll, 1ll, 1ll, 1ll };
    constexpr LVector4 LVectorNegOne{ -1ll, -1ll, -1ll, -1ll };
    constexpr LVector4 LVectorPosInf{ std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max() };
    constexpr LVector4 LVectorNegInf{ std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min() };

    constexpr UVector2 UVector2Zero{ 0u, 0u };
    constexpr UVector2 UVector2One{ 1u, 1u };
    constexpr UVector2 UVector2PosInf{ std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max() };

    constexpr UVector3 UVector3Zero{ 0u, 0u, 0u };
    constexpr UVector3 UVector3One{ 1u, 1u, 1u };
    constexpr UVector3 UVector3PosInf{ std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max() };

    constexpr UVector4 UVector4Zero{ 0u, 0u, 0u, 0u };
    constexpr UVector4 UVector4One{ 1u, 1u, 1u, 1u };
    constexpr UVector4 UVector4PosInf{ std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max() };

    constexpr ULVector2 ULVector2Zero{ 0ull, 0ull };
    constexpr ULVector2 ULVector2One{ 1ull, 1ull };
    constexpr ULVector2 ULVector2PosInf{ std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max() };

    constexpr ULVector3 ULVector3Zero{ 0ull, 0ull, 0ull };
    constexpr ULVector3 ULVector3One{ 1ull, 1ull, 1ull };
    constexpr ULVector3 ULVector3PosInf{ std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max() };

    constexpr ULVector4 ULVector4Zero{ 0ull, 0ul, 0ull, 0ull };
    constexpr ULVector4 ULVector4One{ 1ull, 1ull, 1ull, 1ull };
    constexpr ULVector4 ULVector4PosInf{ std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max() };

    constexpr Matrix2 Matrix2Identity{ 1.0f };
    constexpr Matrix3 Matrix3Identity{ 1.0f };
    constexpr Matrix4 Matrix4Identity{ 1.0f };

    constexpr DMatrix2 DMatrix2Identity{ 1.0 };
    constexpr DMatrix3 DMatrix3Identity{ 1.0 };
    constexpr DMatrix4 DMatrix4Identity{ 1.0 };

    constexpr IMatrix2 IMatrix2Identity{ 1 };
    constexpr IMatrix3 IMatrix3Identity{ 1 };
    constexpr IMatrix4 IMatrix4Identity{ 1 };

    constexpr LMatrix2 LMatrix2Identity{ 1ll };
    constexpr LMatrix3 LMatrix3Identity{ 1ll };
    constexpr LMatrix4 LMatrix4Identity{ 1ll };

    constexpr UMatrix2 UMatrix2Identity{ 1u };
    constexpr UMatrix3 UMatrix3Identity{ 1u };
    constexpr UMatrix4 UMatrix4Identity{ 1u };

    constexpr ULMatrix2 ULMatrix2Identity{ 1ull };
    constexpr ULMatrix3 ULMatrix3Identity{ 1ull };
    constexpr ULMatrix4 ULMatrix4Identity{ 1ull };

    constexpr Quaternion QuaterionIdentity{ 1.0f, 0.0f, 0.0f, 0.0f };
    constexpr DQuaternion DQuaterionIdentity{ 1.0, 0.0, 0.0, 0.0 };

    template <typename T>
    [[nodiscard]] inline auto MapRange(const T value, const Pair<T, T>& inputRange, const Pair<T, T>& outputRange) noexcept -> T
        requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return outputRange.first + ((value - inputRange.first) * (outputRange.second - outputRange.first) / (inputRange.second - inputRange.first));
    }

    [[nodiscard]] extern auto SmoothDamp(const f32 currentValue, const f32 targetValue, f32& smoothing, const f32 smoothingTime, const f32 deltaTime) -> f32;
    [[nodiscard]] extern auto SmoothDamp(const Vector2 currentValue, const Vector2 targetValue, Vector2& smoothing, const f32 smoothingTime, const f32 deltaTime) -> Vector2;
    [[nodiscard]] extern auto SmoothDamp(const Vector3 currentValue, const Vector3 targetValue, Vector3& smoothing, const f32 smoothingTime, const f32 deltaTime) -> Vector3;
    [[nodiscard]] extern auto SmoothDamp(const Vector4 currentValue, const Vector4 targetValue, Vector4& smoothing, const f32 smoothingTime, const f32 deltaTime) -> Vector4;
    [[nodiscard]] extern auto SmoothDamp(const Quaternion& currentValue, const Quaternion& targetValue, Quaternion& smoothing, const f32 smoothingTime, const f32 deltaTime) -> Quaternion;

    [[nodiscard]] extern auto SetMagnitude(const Vector2 vector, const f32 magnitude) -> Vector2;
    [[nodiscard]] extern auto SetMagnitude(const Vector3 vector, const f32 magnitude) -> Vector3;
    [[nodiscard]] extern auto SetMagnitude(const Vector4 vector, const f32 magnitude) -> Vector4;

    [[nodiscard]] extern auto LimitMagnitude(const Vector2 vector, const f32 maxMagnitude) -> Vector2;
    [[nodiscard]] extern auto LimitMagnitude(const Vector3 vector, const f32 maxMagnitude) -> Vector3;
    [[nodiscard]] extern auto LimitMagnitude(const Vector4 vector, const f32 maxMagnitude) -> Vector4;

    [[nodiscard]] extern auto InterpolateAngles(const f32 startAngle, const f32 endAngle, const f32 amount) -> f32;
    [[nodiscard]] extern auto InterpolateAngles(const f32 startAngle, const f32 endAngle, const f32 amount, const i32 additionalSpinCount) -> f32;
}

#endif
