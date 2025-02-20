#pragma once
#include "Component.h"

namespace JE {
	class RigidBody2DComponent : public Component
	{
		using Super = Component;

	public:
		RigidBody2DComponent() : Super(L"RigidBody", ComponentType::RigidBody) {}
		virtual ~RigidBody2DComponent() override {}

		virtual void Init() override;
		virtual void Tick(const float& DeltaTime) override;
		virtual void Render(IRenderer* r) override;

		void SetGround(bool ground) { _bGround = ground; }
		bool IsGround() const { return _bGround; }
		void SetMass(float mass) { _mass = mass; }
		void SetFriction(float friction) { _friction = friction; }
		// 순간적으로 가해지는 힘
		void AddForce(Vector2 force) { _force = force; }
		void SetVelocity(Vector2 velocity) { _velocity = velocity; }
		Vector2 GetVelocity() const { return _velocity; }
		// Vector2(속도 제한, 중력 방향 속도 제한)
		void SetVelocityLimit(Vector2 limit) { _velocityLimit = limit; }
		// Vector2(0, 중력 방향과 중력)
		void SetGravity(Vector2 gravity) { _gravity = gravity; _gravityDir = _gravity.GetNormalize(); }

		void CalculateVelocity(const float& DeltaTime);
		void GravityForce(const float& DeltaTime);
		void LimitVelocity();
		void FrictionForce(const float& DeltaTime);
	private:
		bool _bGround = false;								// 지면에 닿아있는지
		float _mass = 1.f;									// 질량
		float _friction = 10.f;								// 마찰력 (속도의 반대 방향)

		Vector2 _force = Vector2::Zero;						// 힘
		Vector2 _acceleration = Vector2::Zero;				// 가속도 (force / mass)
		Vector2 _velocity = Vector2::Zero;					// 속도 + 방향 = 가속도 * 시간
		Vector2 _velocityLimit = Vector2(200.f, 1000.f);	// 속도 제한 (이 이상 속도가 올라가지 않도록), Vector2(속도 제한, 중력 방향 속도 제한)
		Vector2 _gravity = Vector2(0.f, 800.f);				// 중력 (아래로 향하는 힘), Vecotr2(0, 중력 방향과 중력)
		Vector2 _gravityDir = Vector2::Zero;				// 매번 중력 방향을 계산하기 보다는 저장해서 사용
	};
}
