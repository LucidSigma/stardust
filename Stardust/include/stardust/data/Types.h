#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <cmath>
#include <cstddef>
#include <cstdint>

#include <EASTL/allocator.h>
#include <EASTL/type_traits.h>
#include <EASTL/utility.h>
#include <EASTL/any.h>
#include <EASTL/optional.h>
#include <EASTL/variant.h>

namespace stardust
{
	using i8 = std::int8_t;
	using i16 = std::int16_t;
	using i32 = std::int32_t;
	using i64 = std::int64_t;

	using u8 = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	using ibyte = i8;
	using ubyte = u8;

	using iptr = std::intptr_t;
	using uptr = std::uintptr_t;
	using isize = std::ptrdiff_t;
	using usize = std::size_t;

	using f32 = std::float_t;
	static_assert(sizeof(f32) == 4u);

	using f64 = std::double_t;
	static_assert(sizeof(f64) == 8u);

	template <typename T>
	using Optional = eastl::optional<T>;

	constexpr eastl::nullopt_t NullOpt = eastl::nullopt;

	template <typename... Types>
	using Variant = eastl::variant<Types...>;

	using Any = eastl::any;
}

#endif