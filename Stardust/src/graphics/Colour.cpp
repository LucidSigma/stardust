#include "stardust/graphics/Colour.h"

#include "stardust/data/Types.h"
#include "stardust/utility/random/Random.h"

namespace stardust
{
	[[nodiscard]] Colour CreateColour(const u8 red, const u8 green, const u8 blue)
	{
		return Colour{
			.r = red,
			.g = green,
			.b = blue,
			.a = SDL_ALPHA_OPAQUE,
		};
	}

	[[nodiscard]] Colour CreateColour(const u32 red, const u32 green, const u32 blue)
	{
		return CreateColour(
			static_cast<u8>(red),
			static_cast<u8>(green),
			static_cast<u8>(blue)
		);
	}

	[[nodiscard]] Colour CreateColour(const u8 red, const u8 green, const u8 blue, const u8 alpha)
	{
		return Colour{
			.r = red,
			.g = green,
			.b = blue,
			.a = alpha,
		};
	}

	[[nodiscard]] Colour CreateColour(const u32 red, const u32 green, const u32 blue, const u32 alpha)
	{
		return CreateColour(
			static_cast<u8>(red),
			static_cast<u8>(green),
			static_cast<u8>(blue),
			static_cast<u8>(alpha)
		);
	}

	[[nodiscard]] Colour CreateColour(const f32 red, const f32 green, const f32 blue)
	{
		return Colour{
			.r = static_cast<u8>(red * 255.0f),
			.g = static_cast<u8>(green * 255.0f),
			.b = static_cast<u8>(blue * 255.0f),
			.a = SDL_ALPHA_OPAQUE,
		};
	}

	[[nodiscard]] Colour CreateColour(const f32 red, const f32 green, const f32 blue, const f32 alpha)
	{
		return Colour{
			.r = static_cast<u8>(red * 255.0f),
			.g = static_cast<u8>(green * 255.0f),
			.b = static_cast<u8>(blue * 255.0f),
			.a = static_cast<u8>(alpha * 255.0f),
		};
	}

	[[nodiscard]] HSVColour CreateHSVColour(const f32 hue, const f32 saturation, const f32 value)
	{
		return HSVColour{
			.h = hue,
			.s = saturation,
			.v = value,
			.a = 1.0f,
		};
	}

	[[nodiscard]] HSVColour CreateHSVColour(const f32 hue, const f32 saturation, const f32 value, const f32 alpha)
	{
		return HSVColour{
			.h = hue,
			.s = saturation,
			.v = value,
			.a = alpha,
		};
	}

	[[nodiscard]] Colour RandomColour()
	{
		return Colour{
			.r = static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			.g = static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			.b = static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			.a = static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
		};
	}

	[[nodiscard]] Colour RandomOpaqueColour()
	{
		return Colour{
			.r = static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			.g = static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			.b = static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			.a = SDL_ALPHA_OPAQUE,
		};
	}

	[[nodiscard]] HSVColour RandomHSVColour()
	{
		return HSVColour{
			.h = Random::GenerateFloat(0.0f, 360.0f),
			.s = Random::GenerateFloat(),
			.v = Random::GenerateFloat(),
			.a = Random::GenerateFloat(),
		};
	}

	[[nodiscard]] HSVColour RandomOpaqueHSVColour()
	{
		return HSVColour{
			.h = Random::GenerateFloat(0.0f, 360.0f),
			.s = Random::GenerateFloat(),
			.v = Random::GenerateFloat(),
			.a = 1.0f,
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
			.r = static_cast<u8>(vector.x * 255.0f),
			.g = static_cast<u8>(vector.y * 255.0f),
			.b = static_cast<u8>(vector.z * 255.0f),
			.a = static_cast<u8>(vector.w * 255.0f),
		};
	}

	[[nodiscard]] HSVColour RGBToHSV(const Colour& rgbColour)
	{
		const Vec4 normalisedRGBColour = ColourToVec4(rgbColour);
		const Vec3 hsvColour = glm::hsvColor(Vec3(normalisedRGBColour));

		return CreateHSVColour(hsvColour.x, hsvColour.y, hsvColour.z, normalisedRGBColour.a);
	}

	[[nodiscard]] Colour HSVToRGB(const HSVColour& hsvColour)
	{
		const Vec3 rgbColour = glm::rgbColor(Vec3{ hsvColour.h, hsvColour.s, hsvColour.v });

		return CreateColour(rgbColour.r, rgbColour.g, rgbColour.b, hsvColour.a);
	}
}

[[nodiscard]] bool operator ==(const stardust::Colour& lhs, const stardust::Colour& rhs) noexcept
{
	return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

[[nodiscard]] bool operator !=(const stardust::Colour& lhs, const stardust::Colour& rhs) noexcept
{
	return !(lhs == rhs);
}
