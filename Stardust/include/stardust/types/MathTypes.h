#pragma once
#ifndef STARDUST_MATH_TYPES_H
#define STARDUST_MATH_TYPES_H

#include <glm/glm.hpp>

#include "stardust/types/Primitives.h"

namespace stardust
{
    using Vector1 = glm::tvec1<f32>;
    using Vector2 = glm::tvec2<f32>;
    using Vector3 = glm::tvec3<f32>;
    using Vector4 = glm::tvec4<f32>;

    using DVector1 = glm::tvec1<f64>;
    using DVector2 = glm::tvec2<f64>;
    using DVector3 = glm::tvec3<f64>;
    using DVector4 = glm::tvec4<f64>;

    using IVector1 = glm::tvec1<i32>;
    using IVector2 = glm::tvec2<i32>;
    using IVector3 = glm::tvec3<i32>;
    using IVector4 = glm::tvec4<i32>;

    using LVector1 = glm::tvec1<i64>;
    using LVector2 = glm::tvec2<i64>;
    using LVector3 = glm::tvec3<i64>;
    using LVector4 = glm::tvec4<i64>;

    using UVector1 = glm::tvec1<u32>;
    using UVector2 = glm::tvec2<u32>;
    using UVector3 = glm::tvec3<u32>;
    using UVector4 = glm::tvec4<u32>;

    using ULVector1 = glm::tvec1<u64>;
    using ULVector2 = glm::tvec2<u64>;
    using ULVector3 = glm::tvec3<u64>;
    using ULVector4 = glm::tvec4<u64>;

    template <typename T>
    using TVector1 = glm::tvec1<T>;

    template <typename T>
    using TVector2 = glm::tvec2<T>;

    template <typename T>
    using TVector3 = glm::tvec3<T>;

    template <typename T>
    using TVector4 = glm::tvec4<T>;

    using Matrix2 = glm::tmat2x2<f32>;
    using Matrix3 = glm::tmat3x3<f32>;
    using Matrix4 = glm::tmat4x4<f32>;

    using DMatrix2 = glm::tmat2x2<f64>;
    using DMatrix3 = glm::tmat3x3<f64>;
    using DMatrix4 = glm::tmat4x4<f64>;

    using IMatrix2 = glm::tmat2x2<i32>;
    using IMatrix3 = glm::tmat3x3<i32>;
    using IMatrix4 = glm::tmat4x4<i32>;

    using LMatrix2 = glm::tmat2x2<i64>;
    using LMatrix3 = glm::tmat3x3<i64>;
    using LMatrix4 = glm::tmat4x4<i64>;

    using UMatrix2 = glm::tmat2x2<u32>;
    using UMatrix3 = glm::tmat3x3<u32>;
    using UMatrix4 = glm::tmat4x4<u32>;

    using ULMatrix2 = glm::tmat2x2<u64>;
    using ULMatrix3 = glm::tmat3x3<u64>;
    using ULMatrix4 = glm::tmat4x4<u64>;

    template <usize M, usize N, typename T>
    using TMatrix = glm::mat<M, N, T>;

    using Quaternion = glm::tquat<f32>;
    using DQuaternion = glm::tquat<f64>;

    template <typename T>
    using TQuaternion = glm::tquat<T>;
}

#endif
