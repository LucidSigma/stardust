#include "stardust/physics/collider/Collider.h"

#include "stardust/data/MathTypes.h"
#include "stardust/physics/body/Body.h"
#include "stardust/physics/world/World.h"

namespace stardust
{
	namespace physics
	{
		Collider::Collider(const Body& body, const CreateInfo& createInfo)
			: m_owningBody(&body)
		{
			b2FixtureDef fixtureDef{ };
			fixtureDef.shape = createInfo.shape;
			fixtureDef.friction = createInfo.friction;
			fixtureDef.restitution = createInfo.restitution;
			fixtureDef.restitutionThreshold = createInfo.restitutionThreshold;
			fixtureDef.density = createInfo.density;
			fixtureDef.isSensor = createInfo.isSensor;
			fixtureDef.filter.categoryBits = createInfo.filter.layers;
			fixtureDef.filter.maskBits = createInfo.filter.collidesWith;
			fixtureDef.filter.groupIndex = createInfo.filter.groupIndex;
			fixtureDef.userData = b2FixtureUserData{ };

			m_handle = body.GetRawHandle()->CreateFixture(&fixtureDef);
		}

		Collider::Collider(const ObserverPtr<b2Fixture> fixtureHandle, const World& world)
			: m_handle(fixtureHandle), m_owningBody(world.LookupBody(fixtureHandle->GetBody()))
		{ }

		[[nodiscard]] ShapeType Collider::GetShapeType() const
		{
			return static_cast<ShapeType>(m_handle->GetType());
		}

		[[nodiscard]] ObserverPtr<b2Shape> Collider::GetShape()
		{
			return m_handle->GetShape();
		}

		[[nodiscard]] ObserverPtr<const b2Shape> Collider::GetShape() const
		{
			return m_handle->GetShape();
		}

		[[nodiscard]] f32 Collider::GetFriction() const
		{
			return m_handle->GetFriction();
		}

		void Collider::SetFriction(const f32 friction) const
		{ 
			m_handle->SetFriction(friction);
		}

		[[nodiscard]] f32 Collider::GetRestitution() const
		{
			return m_handle->GetRestitution();
		}

		void Collider::SetRestitution(const f32 restitution) const
		{
			m_handle->SetRestitution(restitution);
		}

		[[nodiscard]] f32 Collider::GetRestitutionThreshold() const
		{
			return m_handle->GetRestitutionThreshold();
		}

		void Collider::SetRestitutionThreshold(const f32 restitutionThreshold) const
		{
			m_handle->SetRestitutionThreshold(restitutionThreshold);
		}

		[[nodiscard]] f32 Collider::GetDensity() const
		{
			return m_handle->GetDensity();
		}

		void Collider::SetDensity(const f32 density) const
		{
			m_handle->SetDensity(density);
		}

		[[nodiscard]] bool Collider::IsSensor() const
		{
			return m_handle->IsSensor();
		}

		void Collider::SetSensor(const bool isSensor) const
		{
			m_handle->SetSensor(isSensor);
		}

		[[nodiscard]] Collider::Filter Collider::GetFilterData() const
		{
			const b2Filter filterData = m_handle->GetFilterData();

			return Filter{
				.layers = filterData.categoryBits,
				.collidesWith = filterData.maskBits,
				.groupIndex = filterData.groupIndex,
			};
		}

		void Collider::SetFilterData(const Filter& filterData) const
		{
			b2Filter filter;
			filter.categoryBits = filterData.layers;
			filter.maskBits = filterData.collidesWith;
			filter.groupIndex = filterData.groupIndex;

			m_handle->SetFilterData(filter);
		}

		void Collider::Refilter() const
		{
			m_handle->Refilter();
		}

		[[nodiscard]] AABB Collider::GetAABB() const
		{
			return m_handle->GetAABB(0);
		}

		[[nodiscard]] MassData Collider::GetMassData() const
		{
			b2MassData massData{ };
			m_handle->GetMassData(&massData);

			return MassData{
				.mass = massData.mass,
				.centre = Vec2{ massData.center.x, massData.center.y },
				.momentOfInertia = massData.I,
			};
		}
	}
}