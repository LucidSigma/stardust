#pragma once
#ifndef RECT_H
#define RECT_H

#include <utility>

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	namespace rect
	{
		using Point = SDL_Point;
		using Rect = SDL_Rect;
		using Line = std::pair<Point, Point>;

		[[nodiscard]] extern Point Create(const i32 x, const i32 y);
		[[nodiscard]] extern Rect Create(const i32 x, const i32 y, const u32 width, const u32 height);
		[[nodiscard]] extern Line Create(const Point& firstPoint, const Point& secondPoint);

		[[nodiscard]] extern bool IsEmpty(const Rect& rect);
		[[nodiscard]] extern u32 GetArea(const Rect& rect);

		[[nodiscard]] extern bool IsAnyPointEnclosed(const Vector<Point>& Points, const Rect& clippingRect);
		[[nodiscard]] extern Rect GetMinimumEnclosingRect(const Vector<Point>& Points);
		[[nodiscard]] extern Rect GetMinimumEnclosingRect(const Vector<Point>& Points, const Rect& clippingRect);

		[[nodiscard]] extern bool IsPointInRect(const Point& Point, const Rect& rect);
		[[nodiscard]] extern bool HasIntersection(const Rect& a, const Rect& b);
		[[nodiscard]] extern bool HasIntersection(const Rect& rect, const Line& line);
		[[nodiscard]] extern Optional<Rect> GetIntersection(const Rect& a, const Rect& b);
		[[nodiscard]] extern Rect GetUnion(const Rect& a, const Rect& b);
	}
}

[[nodiscard]] extern bool operator ==(const stardust::rect::Point& lhs, const stardust::rect::Point& rhs) noexcept;
[[nodiscard]] extern bool operator ==(const stardust::rect::Rect& lhs, const stardust::rect::Rect& rhs) noexcept;
[[nodiscard]] extern bool operator ==(const stardust::rect::Line& lhs, const stardust::rect::Line& rhs) noexcept;

#endif