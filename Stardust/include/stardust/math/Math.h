#pragma once
#ifndef STARDUST_MATH_H
#define STARDUST_MATH_H

#include <cmath>
#include <complex>
#include <limits>
#include <numbers>
#include <ratio>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/transform2.hpp>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
	constexpr Vec2 Vec2Up{ 0.0f, 1.0f };
	constexpr Vec2 Vec2Down{ 0.0f, -1.0f };
	constexpr Vec2 Vec2Left{ -1.0f, 0.0f };
	constexpr Vec2 Vec2Right{ 1.0f, 0.0f };
	constexpr Vec2 Vec2Zero{ 0.0f, 0.0f };
	constexpr Vec2 Vec2One{ 1.0f, 1.0f };
	constexpr Vec2 Vec2NegOne{ -1.0f, -1.0f };
	constexpr Vec2 Vec2PosInf{ std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity() };
	constexpr Vec2 Vec2NegInf{ -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity() };

	constexpr Vec3 Vec3Up{ 0.0f, 1.0f, 0.0f };
	constexpr Vec3 Vec3Down{ 0.0f, -1.0f, 0.0f };
	constexpr Vec3 Vec3Left{ -1.0f, 0.0f, 0.0f };
	constexpr Vec3 Vec3Right{ 1.0f, 0.0f, 0.0f };
	constexpr Vec3 Vec3Forward{ 0.0f, 0.0f, 1.0f };
	constexpr Vec3 Vec3Back{ 0.0f, 0.0f, -1.0f };
	constexpr Vec3 Vec3Zero{ 0.0f, 0.0f, 0.0f };
	constexpr Vec3 Vec3One{ 1.0f, 1.0f, 1.0f };
	constexpr Vec3 Vec3NegOne{ -1.0f, -1.0f, -1.0f };
	constexpr Vec3 Vec3PosInf{ std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity() };
	constexpr Vec3 Vec3NegInf{ -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity() };

	constexpr Vec4 Vec4Zero{ 0.0f, 0.0f, 0.0f, 0.0f };
	constexpr Vec4 Vec4One{ 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr Vec4 Vec4NegOne{ -1.0f, -1.0f, -1.0f, -1.0f };
	constexpr Vec4 Vec4PosInf{ std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity() };
	constexpr Vec4 Vec4NegInf{ -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity() };

	constexpr DVec2 DVec2Up{ 0.0, 1.0 };
	constexpr DVec2 DVec2Down{ 0.0, -1.0 };
	constexpr DVec2 DVec2Left{ -1.0, 0.0 };
	constexpr DVec2 DVec2Right{ 1.0, 0.0 };
	constexpr DVec2 DVec2Zero{ 0.0, 0.0 };
	constexpr DVec2 DVec2One{ 1.0, 1.0 };
	constexpr DVec2 DVec2NegOne{ -1.0, -1.0 };
	constexpr DVec2 DVec2PosInf{ std::numeric_limits<f64>::infinity(), std::numeric_limits<f64>::infinity() };
	constexpr DVec2 DVec2NegInf{ -std::numeric_limits<f64>::infinity(), -std::numeric_limits<f64>::infinity() };

	constexpr DVec3 DVec3Up{ 0.0, 1.0, 0.0 };
	constexpr DVec3 DVec3Down{ 0.0, -1.0, 0.0 };
	constexpr DVec3 DVec3Left{ -1.0, 0.0, 0.0 };
	constexpr DVec3 DVec3Right{ 1.0, 0.0, 0.0 };
	constexpr DVec3 DVec3Forward{ 0.0, 0.0, 1.0 };
	constexpr DVec3 DVec3Back{ 0.0, 0.0, -1.0 };
	constexpr DVec3 DVec3Zero{ 0.0, 0.0, 0.0 };
	constexpr DVec3 DVec3One{ 1.0, 1.0, 1.0 };
	constexpr DVec3 DVec3NegOne{ -1.0, -1.0, -1.0 };
	constexpr DVec3 DVec3PosInf{ std::numeric_limits<f64>::infinity(), std::numeric_limits<f64>::infinity(), std::numeric_limits<f64>::infinity() };
	constexpr DVec3 DVec3NegInf{ -std::numeric_limits<f64>::infinity(), -std::numeric_limits<f64>::infinity(), -std::numeric_limits<f64>::infinity() };

	constexpr DVec4 DVec4Zero{ 0.0, 0.0, 0.0, 0.0 };
	constexpr DVec4 DVec4One{ 1.0, 1.0, 1.0, 1.0 };
	constexpr DVec4 DVec4NegOne{ -1.0, -1.0, -1.0, -1.0 };
	constexpr DVec4 DVec4PosInf{ std::numeric_limits<f64>::infinity(), std::numeric_limits<f64>::infinity(), std::numeric_limits<f64>::infinity(), std::numeric_limits<f64>::infinity() };
	constexpr DVec4 DVec4NegInf{ -std::numeric_limits<f64>::infinity(), -std::numeric_limits<f64>::infinity(), -std::numeric_limits<f64>::infinity(), -std::numeric_limits<f64>::infinity() };

	constexpr IVec2 IVec2Up{ 0, 1 };
	constexpr IVec2 IVec2Down{ 0, -1 };
	constexpr IVec2 IVec2Left{ -1, 0 };
	constexpr IVec2 IVec2Right{ 1, 0 };
	constexpr IVec2 IVec2Zero{ 0, 0 };
	constexpr IVec2 IVec2One{ 1, 1 };
	constexpr IVec2 IVec2NegOne{ -1, -1 };
	constexpr IVec2 IVec2PosInf{ std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max() };
	constexpr IVec2 IVec2NegInf{ std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min() };

	constexpr IVec3 IVec3Up{ 0, 1, 0 };
	constexpr IVec3 IVec3Down{ 0, -1, 0 };
	constexpr IVec3 IVec3Left{ -1, 0, 0 };
	constexpr IVec3 IVec3Right{ 1, 0, 0 };
	constexpr IVec3 IVec3Forward{ 0, 0, 1 };
	constexpr IVec3 IVec3Back{ 0, 0, -1 };
	constexpr IVec3 IVec3Zero{ 0, 0, 0 };
	constexpr IVec3 IVec3One{ 1, 1, 1 };
	constexpr IVec3 IVec3NegOne{ -1, -1, -1 };
	constexpr IVec3 IVec3PosInf{ std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max() };
	constexpr IVec3 IVec3NegInf{ std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min() };

	constexpr IVec4 IVec4Zero{ 0, 0, 0, 0 };
	constexpr IVec4 IVec4One{ 1, 1, 1, 1 };
	constexpr IVec4 IVec4NegOne{ -1, -1, -1, -1 };
	constexpr IVec4 IVec4PosInf{ std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max() };
	constexpr IVec4 IVec4NegInf{ std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min() };

	constexpr LVec2 LVec2Up{ 0l, 1l };
	constexpr LVec2 LVec2Down{ 0l, -1l };
	constexpr LVec2 LVec2Left{ -1l, 0l };
	constexpr LVec2 LVec2Right{ 1l, 0l };
	constexpr LVec2 LVec2Zero{ 0l, 0l };
	constexpr LVec2 LVec2One{ 1l, 1l };
	constexpr LVec2 LVec2NegOne{ -1l, -1l };
	constexpr LVec2 LVec2PosInf{ std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max() };
	constexpr LVec2 LVec2NegInf{ std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min() };

	constexpr LVec3 LVec3Up{ 0l, 1l, 0l };
	constexpr LVec3 LVec3Down{ 0l, -1l, 0l };
	constexpr LVec3 LVec3Left{ -1l, 0l, 0l };
	constexpr LVec3 LVec3Right{ 1l, 0l, 0l };
	constexpr LVec3 LVec3Forward{ 0l, 0l, 1l };
	constexpr LVec3 LVec3Back{ 0l, 0l, -1l };
	constexpr LVec3 LVec3Zero{ 0l, 0l, 0l };
	constexpr LVec3 LVec3One{ 1l, 1l, 1l };
	constexpr LVec3 LVec3NegOne{ -1l, -1l, -1l };
	constexpr LVec3 LVec3PosInf{ std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max() };
	constexpr LVec3 LVec3NegInf{ std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min() };

	constexpr LVec4 LVec4Zero{ 0l, 0l, 0l, 0l };
	constexpr LVec4 LVec4One{ 1l, 1l, 1l, 1l };
	constexpr LVec4 LVec4NegOne{ -1l, -1l, -1l, -1l };
	constexpr LVec4 LVec4PosInf{ std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max() };
	constexpr LVec4 LVec4NegInf{ std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min() };

	constexpr UVec2 UVec2Zero{ 0u, 0u };
	constexpr UVec2 UVec2One{ 1u, 1u };
	constexpr UVec2 UVec2PosInf{ std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max() };

	constexpr UVec3 UVec3Zero{ 0u, 0u, 0u };
	constexpr UVec3 UVec3One{ 1u, 1u, 1u };
	constexpr UVec3 UVec3PosInf{ std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max() };

	constexpr UVec4 UVec4Zero{ 0u, 0u, 0u, 0u };
	constexpr UVec4 UVec4One{ 1u, 1u, 1u, 1u };
	constexpr UVec4 UVec4PosInf{ std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max(), std::numeric_limits<u32>::max() };

	constexpr ULVec2 ULVec2Zero{ 0ul, 0ul };
	constexpr ULVec2 ULVec2One{ 1ul, 1ul };
	constexpr ULVec2 ULVec2PosInf{ std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max() };

	constexpr ULVec3 ULVec3Zero{ 0ul, 0ul, 0ul };
	constexpr ULVec3 ULVec3One{ 1ul, 1ul, 1ul };
	constexpr ULVec3 ULVec3PosInf{ std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max() };

	constexpr ULVec4 ULVec4Zero{ 0ul, 0ul, 0ul, 0ul };
	constexpr ULVec4 ULVec4One{ 1ul, 1ul, 1ul, 1ul };
	constexpr ULVec4 ULVec4PosInf{ std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max() };

	constexpr BVec2 BVec2True{ true, true };
	constexpr BVec2 BVec2False{ false, false };

	constexpr BVec3 BVec3True{ true, true, true };
	constexpr BVec3 BVec3False{ false, false, false };

	constexpr BVec4 BVec4True{ true, true, true, true };
	constexpr BVec4 BVec4False{ false, false, false, false };

	[[nodiscard]] extern f32 SmoothDamp(const f32 currentValue, const f32 targetValue, f32& velocitySmoothing, const f32 smoothingTime, const f32 deltaTime, const f32 maxSpeed = std::numeric_limits<f32>::infinity());
	[[nodiscard]] extern Vec2 SmoothDamp(const Vec2 currentValue, const Vec2 targetValue, Vec2& velocitySmoothing, const f32 smoothingTime, const f32 deltaTime, const f32 maxSpeed = std::numeric_limits<f32>::infinity());
	[[nodiscard]] extern Vec3 SmoothDamp(const Vec3 currentValue, const Vec3 targetValue, Vec3& velocitySmoothing, const f32 smoothingTime, const f32 deltaTime, const f32 maxSpeed = std::numeric_limits<f32>::infinity());
	[[nodiscard]] extern Vec4 SmoothDamp(const Vec4 currentValue, const Vec4 targetValue, Vec4& velocitySmoothing, const f32 smoothingTime, const f32 deltaTime, const f32 maxSpeed = std::numeric_limits<f32>::infinity());
}

#endif