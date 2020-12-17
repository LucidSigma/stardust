#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <EASTL/any.h>
#include <EASTL/optional.h>
#include <EASTL/variant.h>
#include <glad/glad.h>
#include <type_safe/types.hpp>

namespace stardust
{
	using i8 = type_safe::int8_t;
	using i16 = type_safe::int16_t;
	using i32 = type_safe::int32_t;
	using i64 = type_safe::int64_t;

	using u8 = type_safe::uint8_t;
	using u16 = type_safe::uint16_t;
	using u32 = type_safe::uint32_t;
	using u64 = type_safe::uint64_t;

	using iptr = type_safe::intptr_t;
	using uptr = type_safe::uintptr_t;
	using isize = type_safe::ptrdiff_t;
	using usize = type_safe::size_t;

	using f32 = type_safe::float_t;
	static_assert(sizeof(f32) == 4u);

	using f64 = type_safe::double_t;
	static_assert(sizeof(f64) == 8u);

	static_assert(sizeof(GLboolean) == sizeof(bool));

	static_assert(sizeof(GLbyte) == sizeof(i8));
	static_assert(sizeof(GLshort) == sizeof(i16));
	static_assert(sizeof(GLint) == sizeof(i32));
	static_assert(sizeof(GLint64) == sizeof(i64));

	static_assert(sizeof(GLubyte) == sizeof(u8));
	static_assert(sizeof(GLushort) == sizeof(u16));
	static_assert(sizeof(GLuint) == sizeof(u32));
	static_assert(sizeof(GLuint64) == sizeof(u64));

	static_assert(sizeof(GLsizei) == sizeof(i32));
	static_assert(sizeof(GLenum) == sizeof(u32));
	static_assert(sizeof(GLfixed) == sizeof(i32));
	static_assert(sizeof(GLbitfield) == sizeof(u32));

	static_assert(sizeof(GLintptr) == sizeof(iptr));
	static_assert(sizeof(GLsizeiptr) == sizeof(isize));
	static_assert(sizeof(GLsync) == sizeof(iptr));

	static_assert(sizeof(GLhalf) == sizeof(u16));
	static_assert(sizeof(GLfloat) == sizeof(f32));
	static_assert(sizeof(GLclampf) == sizeof(f32));
	static_assert(sizeof(GLdouble) == sizeof(f64));
	static_assert(sizeof(GLclampd) == sizeof(f64));

	template <typename T>
	using Optional = eastl::optional<T>;

	template <typename... Types>
	using Variant = eastl::variant<Types...>;

	using Any = eastl::any;
}

#endif