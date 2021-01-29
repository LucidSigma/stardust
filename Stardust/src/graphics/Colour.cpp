#include "stardust/graphics/Colour.h"

#include "stardust/data/Types.h"
#include "stardust/utility/random/Random.h"

namespace stardust
{
	[[nodiscard]] Colour Colour::Random()
	{
		return Colour(
			static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			static_cast<u8>(Random::GenerateInt<u32>(0u, 255u))
		);
	}

	[[nodiscard]] Colour Colour::RandomOpaque()
	{
		return Colour(
			static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			static_cast<u8>(Random::GenerateInt<u32>(0u, 255u)),
			static_cast<u8>(Random::GenerateInt<u32>(0u, 255u))
		);
	}

	[[nodiscard]] Colour::operator SDL_Colour() const noexcept
	{
		return SDL_Colour{
			.r = red,
			.g = green,
			.b = blue,
			.a = alpha,
		};
	}

	[[nodiscard]] Colour::operator Vec3() const noexcept
	{
		return Vec3{
			static_cast<f32>(red) / 255.0f,
			static_cast<f32>(green) / 255.0f,
			static_cast<f32>(blue) / 255.0f,
		};
	}

	[[nodiscard]] Colour::operator Vec4() const noexcept
	{
		return Vec4{
			static_cast<f32>(red) / 255.0f,
			static_cast<f32>(green) / 255.0f,
			static_cast<f32>(blue) / 255.0f,
			static_cast<f32>(alpha) / 255.0f,
		};
	}

	[[nodiscard]] HSVColour HSVColour::Random()
	{
		return HSVColour(
			Random::GenerateFloat(0.0f, 360.0f),
			Random::GenerateFloat(),
			Random::GenerateFloat(),
			Random::GenerateFloat()
		);
	}

	[[nodiscard]] HSVColour HSVColour::RandomOpaque()
	{
		return HSVColour(
			Random::GenerateFloat(0.0f, 360.0f),
			Random::GenerateFloat(),
			Random::GenerateFloat()
		);
	}

	[[nodiscard]] HSVColour::operator Vec3() const noexcept
	{
		return Vec3{ hue, saturation, value };
	}

	[[nodiscard]] HSVColour::operator Vec4() const noexcept
	{
		return Vec4{ hue, saturation, value, alpha };
	}

	[[nodiscard]] HSVColour RGBToHSV(const Colour& rgbColour)
	{
		const Vec4 normalisedRGBColour = Vec4(rgbColour);
		const Vec3 hsvColour = glm::hsvColor(Vec3(normalisedRGBColour));

		return HSVColour(hsvColour.x, hsvColour.y, hsvColour.z, normalisedRGBColour.a);
	}

	[[nodiscard]] Colour HSVToRGB(const HSVColour& hsvColour)
	{
		const Vec3 rgbColour = glm::rgbColor(Vec3{ hsvColour.hue, hsvColour.saturation, hsvColour.value });

		return Colour(rgbColour.r, rgbColour.g, rgbColour.b, hsvColour.alpha);
	}

	[[nodiscard]] Colour HexToRGB(u32 colourHex, const u8 alpha)
	{
		constexpr u32 bitmask = 0xFF;
		Colour colour;

		colour.blue = static_cast<u8>(colourHex & bitmask);
		colourHex >>= sizeof(u8) * 8u;

		colour.green = static_cast<u8>(colourHex & bitmask);
		colourHex >>= sizeof(u8) * 8u;

		colour.red = static_cast<u8>(colourHex & bitmask);
		colour.alpha = alpha;

		return colour;
	}

	[[nodiscard]] u32 RGBToHex(const Colour& rgbColour)
	{
		u32 colourHex = 0u;

		colourHex |= static_cast<u32>(rgbColour.red);
		colourHex <<= sizeof(u8) * 8u;

		colourHex |= static_cast<u32>(rgbColour.green);
		colourHex <<= sizeof(u8) * 8u;

		colourHex |= static_cast<u32>(rgbColour.blue);

		return colourHex;
	}
}

[[nodiscard]] stardust::Colour operator ""_colour(const stardust::u64 hexCode)
{
	return stardust::HexToRGB(static_cast<stardust::u32>(hexCode));
}