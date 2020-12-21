#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Rect functions can determine basic collisions with points, lines, and other rectangles", "[rect]")
{
	SECTION("Can test for equality between shapes")
	{
		REQUIRE(sd::rect::Create(0, 0) == sd::rect::Create(0, 0));
		REQUIRE(sd::rect::Create(0, 1) == sd::rect::Create(0, 1));
		REQUIRE(sd::rect::Create(0, 1) != sd::rect::Create(1, 0));

		REQUIRE(sd::rect::Create(0, 0, 2u, 2u) == sd::rect::Create(0, 0, 2u, 2u));
		REQUIRE(sd::rect::Create(0, 1, 3u, 6u) == sd::rect::Create(0, 1, 3u, 6u));
		REQUIRE(sd::rect::Create(0, 1, 1u, 1u) != sd::rect::Create(1, 0, 1u, 1u));

		REQUIRE(sd::rect::Create({ 0, 0 }, { 1, 1 }) == sd::rect::Create({ 0, 0 }, { 1, 1 }));
		REQUIRE(sd::rect::Create({ 0, 1 }, { 1, 0 }) == sd::rect::Create({ 0, 1 }, { 1, 0 }));
		REQUIRE(sd::rect::Create({ 0, 1 }, { 1, 0 }) != sd::rect::Create({ 1, 0 }, { 0, 1 }));
	}

	SECTION("Can query information about the area of a rectangle")
	{
		const auto rectA = sd::rect::Create(1, 1, 10u, 10u);
		const auto rectB = sd::rect::Create(1, 1, 10u, 0u);

		REQUIRE(sd::rect::GetArea(rectA) == 100u);
		REQUIRE(sd::rect::GetArea(rectB) == 0u);

		REQUIRE(!sd::rect::IsEmpty(rectA));
		REQUIRE(sd::rect::IsEmpty(rectB));
	}

	SECTION("Can determine if points are enclosed in a rectangle")
	{
		REQUIRE(sd::rect::IsPointInRect(sd::rect::Create(1, 1), sd::rect::Create(0, 0, 2u, 2u)));
		REQUIRE(!sd::rect::IsPointInRect(sd::rect::Create(3, 3), sd::rect::Create(0, 0, 2u, 2u)));

		const sd::Vector<sd::rect::Point> points{
			sd::rect::Create(1, 1),
			sd::rect::Create(1, 2),
			sd::rect::Create(1, 3),
			sd::rect::Create(2, 1),
			sd::rect::Create(2, 2),
			sd::rect::Create(2, 3),
		};

		REQUIRE(sd::rect::IsAnyPointEnclosed(points, sd::rect::Create(0, 0, 3u, 3u)));
		REQUIRE(!sd::rect::IsAnyPointEnclosed(points, sd::rect::Create(3, 3, 1u, 1u)));
	}

	SECTION("Can create minimum enclosing rectangles around given points")
	{
		const sd::Vector<sd::rect::Point> points{
			sd::rect::Create(1, 1),
			sd::rect::Create(2, 2),
			sd::rect::Create(3, 3),
		};

		REQUIRE(sd::rect::GetMinimumEnclosingRect(points) == sd::rect::Create(1, 1, 3u, 3u));

		const auto clippingRect = sd::rect::Create(1, 1, 1u, 1u);
		REQUIRE(sd::rect::GetMinimumEnclosingRect(points, clippingRect) == sd::rect::Create(1, 1, 1u, 1u));
	}

	SECTION("Can resolve basic collisions between rectangles")
	{
		REQUIRE(sd::rect::HasIntersection(sd::rect::Create(0, 0, 2u, 2u), sd::rect::Create(1, 1, 2u, 2u)));
		REQUIRE(!sd::rect::HasIntersection(sd::rect::Create(0, 0, 2u, 2u), sd::rect::Create(3, 3, 2u, 2u)));

		REQUIRE(sd::rect::HasIntersection(sd::rect::Create(0, 0, 2u, 2u), sd::rect::Create({ -1, -1 }, { 3, 3 })));
		REQUIRE(!sd::rect::HasIntersection(sd::rect::Create(0, 0, 2u, 2u), sd::rect::Create({ -2, -2 }, { -1, -1 })));

		REQUIRE(sd::rect::GetIntersection(sd::rect::Create(0, 0, 2u, 2u), sd::rect::Create(1, 1, 2u, 2u)).value() == sd::rect::Create(1, 1, 1u, 1u));
		REQUIRE(sd::rect::GetUnion(sd::rect::Create(0, 0, 2u, 2u), sd::rect::Create(1, 1, 2u, 2u)) == sd::rect::Create(0, 0, 3u, 3u));
	}
}