#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Rects can be created and have information queried about them", "[rect]")
{
    sd::Rect rectA(0, 0, 10u, 20u);
    sd::Rect rectB(-20, 30, 45u, 17u);
    sd::Rect rectC(5, 5, 7u, 7u);

    SECTION("Can query basic information about rects")
    {
        REQUIRE(rectA.GetTopLeft() == sd::IVec2{ 0, 0 });
        REQUIRE(rectA.GetBottomRight() == sd::IVec2{ 10, 20 });
        REQUIRE(rectA.GetCentre() == sd::IVec2{ 5, 10 });

        REQUIRE(rectB.GetTopLeft() == sd::IVec2{ -20, 30 });
        REQUIRE(rectB.GetBottomRight() == sd::IVec2{ 25, 47 });
        REQUIRE(rectB.GetCentre() == sd::IVec2{ 2, 38 });

        REQUIRE(rectC.GetTopLeft() == sd::IVec2{ 5, 5 });
        REQUIRE(rectC.GetBottomRight() == sd::IVec2{ 12, 12 });
        REQUIRE(rectC.GetCentre() == sd::IVec2{ 8, 8 });

        REQUIRE(rectA.GetArea() == 200u);
        REQUIRE(rectB.GetArea() == 765u);
        REQUIRE(rectC.GetArea() == 49u);

        REQUIRE(rectA.GetPerimeter() == 60u);
        REQUIRE(rectB.GetPerimeter() == 124u);
        REQUIRE(rectC.GetPerimeter() == 28u);

        REQUIRE(22.361f - rectA.GetDiagonal() < 0.01f);
        REQUIRE(48.105f - rectB.GetDiagonal() < 0.01f);
        REQUIRE(9.9f - rectC.GetDiagonal() < 0.01f);

        REQUIRE(!rectA.IsSquare());
        REQUIRE(!rectB.IsSquare());
        REQUIRE(rectC.IsSquare());

        REQUIRE(sd::Rect(10, 10, 0u, 10u).IsEmpty());
    }

    SECTION("Can determine if points are enclosed in a rect")
    {
        REQUIRE(rectA.IsPointEnclosed(sd::IVec2{ 5, 5 }));
        REQUIRE(!rectA.IsPointEnclosed(sd::IVec2{ 15, 15 }));

        const sd::Vector<sd::IVec2> points{
            sd::IVec2{ 8, 10 },
            sd::IVec2{ 20, 20 },
            sd::IVec2{ 25, 25 },
        };

        REQUIRE(rectA.IsAnyPointEnclosed(points));
        REQUIRE(!rectB.IsAnyPointEnclosed(points));
        REQUIRE(rectC.IsAnyPointEnclosed(points));

        REQUIRE(sd::Rect::GetMinimumEnslosingRect(points) == sd::Rect(8, 10, 18u, 16u));

        const sd::Rect clippingRect(18, 18, 10, 10);
        REQUIRE(sd::Rect::GetMinimumEnslosingRect(points, clippingRect) == sd::Rect(20, 20, 6u, 6u));
    }
}