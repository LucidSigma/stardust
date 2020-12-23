#include "stardust/graphics/Colour.h"

#include "stardust/data/Types.h"

namespace stardust
{
	[[nodiscard]] Colour CreateColour(const u8 red, const u8 green, const u8 blue)
	{
		return Colour{
			red, green, blue, SDL_ALPHA_OPAQUE
		};
	}

	[[nodiscard]] Colour CreateColour(const u8 red, const u8 green, const u8 blue, const u8 alpha)
	{
		return Colour{
			red, green, blue, alpha
		};
	}

	[[nodiscard]] Colour CreateColour(const f32 red, const f32 green, const f32 blue)
	{
		return Colour{
			static_cast<u8>(red * 255.0f),
			static_cast<u8>(green * 255.0f),
			static_cast<u8>(blue * 255.0f),
			SDL_ALPHA_OPAQUE,
		};
	}

	[[nodiscard]] Colour CreateColour(const f32 red, const f32 green, const f32 blue, const f32 alpha)
	{
		return Colour{
			static_cast<u8>(red * 255.0f),
			static_cast<u8>(green * 255.0f),
			static_cast<u8>(blue * 255.0f),
			static_cast<u8>(alpha * 255.0f),
		};
	}

	[[nodiscard]] Vec4 ColourToVec4(const Colour& colour)
	{
		return Vec4{
			static_cast<f32>(colour.r) / 255.0f,
			static_cast<f32>(colour.g) / 255.0f,
			static_cast<f32>(colour.b) / 255.0f,
			static_cast<f32>(colour.a) / 255.0f,
		};
	}

	[[nodiscard]] Colour Vec4ToColour(const Vec4& vector)
	{
		return Colour{
			static_cast<u8>(vector.x * 255.0f),
			static_cast<u8>(vector.y * 255.0f),
			static_cast<u8>(vector.z * 255.0f),
			static_cast<u8>(vector.w * 255.0f),
		};
	}
}