#pragma once
#ifndef STARDUST_CHAIN_H
#define STARDUST_CHAIN_H

#include <box2d/box2d.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/physics/geometry/Edge.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
	namespace physics
	{
		class Chain
		{
		private:
			b2ChainShape m_chain{ };

		public:
			Chain() = default;
			Chain(const b2ChainShape& shapeHandle);
			Chain(const Vector<Vec2>& vertices);
			Chain(const Vector<Vec2>& vertices, const Vec2& ghostVertexA, const Vec2& ghostVertexB);
			~Chain() noexcept = default;

			void CreateLoop(const Vector<Vec2>& vertices);
			void CreateChain(const Vector<Vec2>& vertices, const Vec2& ghostVertexA, const Vec2& ghostVertexB);

			void Clear();

			[[nodiscard]] Vector<Vec2> GetVertices() const;
			inline u32 GetVertexCount() const noexcept { return static_cast<u32>(m_chain.m_count); }
			[[nodiscard]] Pair<Vec2, Vec2> GetGhostVertices() const;

			[[nodiscard]] Edge GetChildEdge(const u32 childEdgeIndex) const;
			inline u32 GetChildEdgeCount() const noexcept { return static_cast<u32>(m_chain.GetChildCount()); }			

			[[nodiscard]] bool TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const;
			[[nodiscard]] AABB ComputeAABB(const Vec2& worldPosition, const f32 rotation) const;
			[[nodiscard]] MassData ComputeMassData(const f32 density) const;

			inline operator ObserverPtr<b2Shape>() noexcept { return &m_chain; }
			inline operator ObserverPtr<const b2Shape>() const noexcept { return &m_chain; }
			inline operator ObserverPtr<b2ChainShape>() noexcept { return &m_chain; }
			inline operator ObserverPtr<const b2ChainShape>() const noexcept { return &m_chain; }
		};
	}
}

#endif