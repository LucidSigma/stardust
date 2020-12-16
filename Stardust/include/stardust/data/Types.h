#include <tinyutf8/tinyutf8.h>
#include <type_safe/types.hpp>

namespace stardust
{
	namespace ts = type_safe;

	using i8 = ts::int8_t;
	using i16 = ts::int16_t;
	using i32 = ts::int32_t;
	using i64 = ts::int64_t;

	using u8 = ts::uint8_t;
	using u16 = ts::uint16_t;
	using u32 = ts::uint32_t;
	using u64 = ts::uint64_t;

	using iptr = ts::intptr_t;
	using uptr = ts::uintptr_t;
	using isize = ts::ptrdiff_t;
	using usize = ts::size_t;

	using f32 = ts::float_t;
	static_assert(sizeof(f32) == 4u);

	using f64 = ts::double_t;
	static_assert(sizeof(f64) == 8u);

	// TODO: Add static assertions for OpenGL types.

	using String = tiny_utf8::utf8_string;
}