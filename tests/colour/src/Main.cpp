#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Colours can be created and converted between differen colour spaces", "[colour]")
{
	SECTION("Can create colours in RGB space")
	{
		 sd::Colour colour(34u, 189u, 197u);
		 REQUIRE(colour.red == 34u);
		 REQUIRE(colour.green == 189u);
		 REQUIRE(colour.blue == 197u);
		 REQUIRE(colour.alpha == 255u);

		 colour = sd::Colour(0.5f, 0.25f, 0.3f, 0.7f);
		 REQUIRE(colour.red == 127u);
		 REQUIRE(colour.green == 63u);
		 REQUIRE(colour.blue == 76u);
		 REQUIRE(colour.alpha == 178u);
	}

	SECTION("Can create colours in HSV space")
	{
		const sd::HSVColour colour(195.0f, 0.5f, 0.3f);
		REQUIRE(colour.hue == 195.0f);
		REQUIRE(colour.saturation == 0.5f);
		REQUIRE(colour.value == 0.3f);
		REQUIRE(colour.alpha == 1.0f);
	}

	SECTION("Can convert colours to SDL_Colour structs")
	{
		const sd::Colour colour(128u, 45u, 174u, 200u);
		const SDL_Colour sdlColour = colour;

		REQUIRE(sdlColour.r == 128u);
		REQUIRE(sdlColour.g == 45u);
		REQUIRE(sdlColour.b == 174u);
		REQUIRE(sdlColour.a == 200u);
	}

	SECTION("Can convert colours to math vectors")
	{
		const sd::Colour colour(0u, 255u, 255u);
		const sd::Vec3 colourVec3(colour);
		const sd::Vec4 colourVec4(colour);

		REQUIRE(colourVec3.r == 0.0f);
		REQUIRE(colourVec3.g == 1.0f);
		REQUIRE(colourVec3.b == 1.0f);
		REQUIRE(colourVec4.r == 0.0f);
		REQUIRE(colourVec4.b == 1.0f);
		REQUIRE(colourVec4.g == 1.0f);
		REQUIRE(colourVec4.a == 1.0f);

		const sd::HSVColour hsvColour(145.0f, 0.45f, 0.75f);
		const sd::Vec3 hsvColourVec3(hsvColour);
		const sd::Vec4 hsvColourVec4(hsvColour);

		REQUIRE(hsvColourVec3.r == 145.0f);
		REQUIRE(hsvColourVec3.g == 0.45f);
		REQUIRE(hsvColourVec3.b == 0.75f);
		REQUIRE(hsvColourVec4.r == 145.0f);
		REQUIRE(hsvColourVec4.g == 0.45f);
		REQUIRE(hsvColourVec4.b == 0.75f);
		REQUIRE(hsvColourVec4.a == 1.0f);
	}

	SECTION("Can convert colours between RGB and HSV colour spaces")
	{
		const sd::Colour rgbColour(64u, 89u, 102u);
		const sd::HSVColour convertedRGBColour = sd::RGBToHSV(rgbColour);

		REQUIRE(glm::round(convertedRGBColour.hue) == 201.0f);
		REQUIRE(glm::round(convertedRGBColour.saturation * 1'000.0f) / 1'000.0f == 0.373f);
		REQUIRE(convertedRGBColour.value == 0.4f);
		REQUIRE(convertedRGBColour.alpha == 1.0f);

		const sd::HSVColour hsvColour(67.0f, 0.38f, 0.9f);
		const sd::Colour convertedHSVColour = sd::HSVToRGB(hsvColour);

		REQUIRE(convertedHSVColour.red == 219u);
		REQUIRE(convertedHSVColour.green == 229u);
		REQUIRE(convertedHSVColour.blue == 142u);
		REQUIRE(convertedHSVColour.alpha == 255u);
	}

	SECTION("Can convert RGB colours to and from hex codes")
	{
		const sd::Colour colour = 0xF49E76_colour;
		REQUIRE(colour.red == 244u);
		REQUIRE(colour.green == 158u);
		REQUIRE(colour.blue == 118u);
		REQUIRE(colour.alpha == 255u);

		const sd::u32 hex = sd::RGBToHex(sd::Colour(157u, 23u, 98u));
		REQUIRE(hex == 0x9D1762u);
	}
}