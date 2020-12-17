#pragma once
#ifndef MATH_TYPES_H
#define MATH_TYPES_H

#include <glm/glm.hpp>

#include "Types.h"

namespace stardust
{
	using Vec1 = glm::tvec1<f32>;
	using Vec2 = glm::tvec2<f32>;
	using Vec3 = glm::tvec3<f32>;
	using Vec4 = glm::tvec4<f32>;

	using DVec1 = glm::tvec1<f64>;
	using DVec2 = glm::tvec2<f64>;
	using DVec3 = glm::tvec3<f64>;
	using DVec4 = glm::tvec4<f64>;

	using IVec1 = glm::tvec1<i32>;
	using IVec2 = glm::tvec2<i32>;
	using IVec3 = glm::tvec3<i32>;
	using IVec4 = glm::tvec4<i32>;

	using I64Vec1 = glm::tvec1<i64>;
	using I64Vec2 = glm::tvec2<i64>;
	using I64Vec3 = glm::tvec3<i64>;
	using I64Vec4 = glm::tvec4<i64>;

	using UVec1 = glm::tvec1<u32>;
	using UVec2 = glm::tvec2<u32>;
	using UVec3 = glm::tvec3<u32>;
	using UVec4 = glm::tvec4<u32>;

	using U64Vec1 = glm::tvec1<u64>;
	using U64Vec2 = glm::tvec2<u64>;
	using U64Vec3 = glm::tvec3<u64>;
	using U64Vec4 = glm::tvec4<u64>;

	using BVec1 = glm::tvec1<bool>;
	using BVec2 = glm::tvec2<bool>;
	using BVec3 = glm::tvec3<bool>;
	using BVec4 = glm::tvec4<bool>;

	using Mat2 = glm::tmat2x2<f32>;
	using Mat2x2 = glm::tmat2x2<f32>;
	using Mat2x3 = glm::tmat2x3<f32>;
	using Mat2x4 = glm::tmat2x4<f32>;
	using Mat3 = glm::tmat3x3<f32>;
	using Mat3x3 = glm::tmat3x3<f32>;
	using Mat3x2 = glm::tmat3x2<f32>;
	using Mat3x4 = glm::tmat3x4<f32>;
	using Mat4 = glm::tmat4x4<f32>;
	using Mat4x4 = glm::tmat4x4<f32>;
	using Mat4x2 = glm::tmat4x2<f32>;
	using Mat4x3 = glm::tmat4x3<f32>;

	using DMat2 = glm::tmat2x2<f64>;
	using DMat2x2 = glm::tmat2x2<f64>;
	using DMat2x3 = glm::tmat2x3<f64>;
	using DMat2x4 = glm::tmat2x4<f64>;
	using DMat3 = glm::tmat3x3<f64>;
	using DMat3x3 = glm::tmat3x3<f64>;
	using DMat3x2 = glm::tmat3x2<f64>;
	using DMat3x4 = glm::tmat3x4<f64>;
	using DMat4 = glm::tmat4x4<f64>;
	using DMat4x4 = glm::tmat4x4<f64>;
	using DMat4x2 = glm::tmat4x2<f64>;
	using DMat4x3 = glm::tmat4x3<f64>;

	using IMat2 = glm::tmat2x2<i32>;
	using IMat2x2 = glm::tmat2x2<i32>;
	using IMat2x3 = glm::tmat2x3<i32>;
	using IMat2x4 = glm::tmat2x4<i32>;
	using IMat3 = glm::tmat3x3<i32>;
	using IMat3x3 = glm::tmat3x3<i32>;
	using IMat3x2 = glm::tmat3x2<i32>;
	using IMat3x4 = glm::tmat3x4<i32>;
	using IMat4 = glm::tmat4x4<i32>;
	using IMat4x4 = glm::tmat4x4<i32>;
	using IMat4x2 = glm::tmat4x2<i32>;
	using IMat4x3 = glm::tmat4x3<i32>;

	using I64Mat2 = glm::tmat2x2<i64>;
	using I64Mat2x2 = glm::tmat2x2<i64>;
	using I64Mat2x3 = glm::tmat2x3<i64>;
	using I64Mat2x4 = glm::tmat2x4<i64>;
	using I64Mat3 = glm::tmat3x3<i64>;
	using I64Mat3x3 = glm::tmat3x3<i64>;
	using I64Mat3x2 = glm::tmat3x2<i64>;
	using I64Mat3x4 = glm::tmat3x4<i64>;
	using I64Mat4 = glm::tmat4x4<i64>;
	using I64Mat4x4 = glm::tmat4x4<i64>;
	using I64Mat4x2 = glm::tmat4x2<i64>;
	using I64Mat4x3 = glm::tmat4x3<i64>;

	using UMat2 = glm::tmat2x2<u32>;
	using UMat2x2 = glm::tmat2x2<u32>;
	using UMat2x3 = glm::tmat2x3<u32>;
	using UMat2x4 = glm::tmat2x4<u32>;
	using UMat3 = glm::tmat3x3<u32>;
	using UMat3x3 = glm::tmat3x3<u32>;
	using UMat3x2 = glm::tmat3x2<u32>;
	using UMat3x4 = glm::tmat3x4<u32>;
	using UMat4 = glm::tmat4x4<u32>;
	using UMat4x4 = glm::tmat4x4<u32>;
	using UMat4x2 = glm::tmat4x2<u32>;
	using UMat4x3 = glm::tmat4x3<u32>;

	using U64Mat2 = glm::tmat2x2<u64>;
	using U64Mat2x2 = glm::tmat2x2<u64>;
	using U64Mat2x3 = glm::tmat2x3<u64>;
	using U64Mat2x4 = glm::tmat2x4<u64>;
	using U64Mat3 = glm::tmat3x3<u64>;
	using U64Mat3x3 = glm::tmat3x3<u64>;
	using U64Mat3x2 = glm::tmat3x2<u64>;
	using U64Mat3x4 = glm::tmat3x4<u64>;
	using U64Mat4 = glm::tmat4x4<u64>;
	using U64Mat4x4 = glm::tmat4x4<u64>;
	using U64Mat4x2 = glm::tmat4x2<u64>;
	using U64Mat4x3 = glm::tmat4x3<u64>;

	using Quaternion = glm::tquat<f32>;
	using DQuaternion = glm::tquat<f64>;
}

#endif