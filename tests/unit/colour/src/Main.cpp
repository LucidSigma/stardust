#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <stardust/Stardust.h>

TEST_CASE("Colours can be created and converted", "[colour]")
{
    SECTION("Can create RGBA colours")
    {
        sd::Colour colour(34u, 189u, 197u);
        REQUIRE(colour.red == 34u);
        REQUIRE(colour.green == 189u);
        REQUIRE(colour.blue == 197u);
        REQUIRE(colour.alpha == sd::Colour::OpaqueChannel);

        colour = sd::Colour(0.5f, 0.25f, 0.3f, 0.7f);
        REQUIRE(colour.red == 127u);
        REQUIRE(colour.green == 63u);
        REQUIRE(colour.blue == 76u);
        REQUIRE(colour.alpha == 178u);
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
        const sd::Vector3 colourVec3(colour);
        const sd::Vector4 colourVec4(colour);

        REQUIRE(colourVec3.r == 0.0f);
        REQUIRE(colourVec3.g == 1.0f);
        REQUIRE(colourVec3.b == 1.0f);

        REQUIRE(colourVec4.r == 0.0f);
        REQUIRE(colourVec4.b == 1.0f);
        REQUIRE(colourVec4.g == 1.0f);
        REQUIRE(colourVec4.a == 1.0f);
    }

    SECTION("Can convert RGB colours to and from hex codes")
    {
        using namespace sd::literals;

        const sd::Colour colour = 0xF49E76_colour;
        REQUIRE(colour.red == 244u);
        REQUIRE(colour.green == 158u);
        REQUIRE(colour.blue == 118u);
        REQUIRE(colour.alpha == 255u);

        const sd::u32 hex = sd::Colour(157u, 23u, 98u).GetHex();
        REQUIRE(hex == 0x9D1762u);
    }

    SECTION("Can convert colours between RGB and HSV colour spaces")
    {
        const sd::Colour colour(64u, 89u, 102u);
        const sd::Colour::HSV hsvValues = colour.GetHSV();

        REQUIRE(glm::round(hsvValues.hue) == 201.0f);
        REQUIRE(glm::round(hsvValues.saturation * 1'000.0f) / 1'000.0f == 0.373f);
        REQUIRE(hsvValues.value == 0.4f);

        const sd::Colour fromHSV(sd::Colour::HSV{ 67.0f, 0.38f, 0.9f });

        REQUIRE(fromHSV.red == 219u);
        REQUIRE(fromHSV.green == 229u);
        REQUIRE(fromHSV.blue == 142u);
        REQUIRE(fromHSV.alpha == 255u);
    }
}
