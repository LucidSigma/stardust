#pragma once
#ifndef STARDUST_POLYGON_H
#define STARDUST_POLYGON_H

#include <box2d/box2d.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
	namespace physics
	{
		class Polygon
		{
		private:
			b2PolygonShape m_polygon{ };

		public:
			static constexpr u32 GetMaxVertices() noexcept { return b2_maxPolygonVertices; }

			Polygon() = default;
			Polygon(const b2PolygonShape& shapeHandle);
			Polygon(const Vector<Vec2>& points);
			~Polygon() noexcept = default;

			void Set(const Vector<Vec2>& points);
			void SetAsBox(const f32 width, const f32 height);
			void SetAsBox(const f32 width, const f32 height, const Vec2& pivot, const f32 rotation);

			[[nodiscard]] Vec2 GetCentroid() const noexcept;
			[[nodiscard]] Vector<Vec2> GetVertices() const;
			[[nodiscard]] Vector<Vec2> GetNormals() const;
			inline u32 GetVertexCount() const noexcept { return static_cast<u32>(m_polygon.m_count); }

			[[nodiscard]] bool TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const;
			[[nodiscard]] AABB ComputeAABB(const Vec2& worldPosition, const f32 rotation) const;
			[[nodiscard]] MassData ComputeMassData(const f32 density) const;

			[[nodiscard]] bool IsConvex() const;

			inline operator ObserverPtr<b2Shape>() noexcept { return &m_polygon; }
			inline operator ObserverPtr<const b2Shape>() const noexcept { return &m_polygon; }
			inline operator ObserverPtr<b2PolygonShape>() noexcept { return &m_polygon; }
			inline operator ObserverPtr<const b2PolygonShape>() const noexcept { return &m_polygon; }
		};
	}
}

#endif