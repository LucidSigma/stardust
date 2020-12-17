#include "stardust/rect/Rect.h"

namespace stardust
{
	namespace rect
	{
		[[nodiscard]] Point Create(const i32 x, const i32 y)
		{
			return Point{ x, y };
		}

		[[nodiscard]] Rect Create(const i32 x, const i32 y, const u32 width, const u32 height)
		{
			return Rect{ x, y, static_cast<i32>(width), static_cast<i32>(height) };
		}

		[[nodiscard]] Line Create(const Point& firstPoint, const Point& secondPoint)
		{
			return Line{ firstPoint, secondPoint };
		}

		[[nodiscard]] bool IsEmpty(const Rect& rect)
		{
			return static_cast<bool>(SDL_RectEmpty(&rect));
		}

		[[nodiscard]] u32 GetArea(const Rect& rect)
		{
			return rect.w * rect.h;
		}

		[[nodiscard]] bool IsAnyPointEnclosed(const Vector<Point>& points, const Rect& clippingRect)
		{
			return static_cast<bool>(SDL_EnclosePoints(points.data(), static_cast<i32>(points.size()), &clippingRect, nullptr));
		}

		[[nodiscard]] Rect GetMinimumEnclosingRect(const Vector<Point>& points)
		{
			Rect minimumEnclosingRect{ };
			SDL_EnclosePoints(points.data(), static_cast<i32>(points.size()), nullptr, &minimumEnclosingRect);

			return minimumEnclosingRect;
		}

		[[nodiscard]] Rect GetMinimumEnclosingRect(const Vector<Point>& points, const Rect& clippingRect)
		{
			Rect minimumEnclosingRect{ };
			SDL_EnclosePoints(points.data(), static_cast<i32>(points.size()), &clippingRect, &minimumEnclosingRect);

			return minimumEnclosingRect;
		}

		[[nodiscard]] bool IsPointInRect(const Point& point, const Rect& rect)
		{
			return static_cast<bool>(SDL_PointInRect(&point, &rect));
		}

		[[nodiscard]] bool HasIntersection(const Rect& a, const Rect& b)
		{
			return static_cast<bool>(SDL_HasIntersection(&a, &b));
		}

		[[nodiscard]] bool HasIntersection(const Rect& rect, const Line& line)
		{
			Line lineCopy = line;

			return static_cast<bool>(SDL_IntersectRectAndLine(&rect, &lineCopy.first.x, &lineCopy.first.y, &lineCopy.second.x, &lineCopy.second.y));
		}

		[[nodiscard]] Optional<Rect> GetIntersection(const Rect& a, const Rect& b)
		{
			Rect intersection{ };
			const bool doesIntersectionExist = static_cast<bool>(SDL_IntersectRect(&a, &b, &intersection));

			return doesIntersectionExist ? Optional<Rect>(intersection) : NullOpt;
		}

		[[nodiscard]] Rect GetUnion(const Rect& a, const Rect& b)
		{
			Rect rectUnion{ };
			SDL_UnionRect(&a, &b, &rectUnion);

			return rectUnion;
		}

		void ClipLineToRect(const Rect& rect, Line& line)
		{
			SDL_IntersectRectAndLine(&rect, &line.first.x, &line.first.y, &line.second.x, &line.second.y);
		}
	}
}