#pragma once
#include "Component.h"

namespace JE {
    class RigidBodyComponent : public Component
    {
		using Super = Component;
	public:
		RigidBodyComponent() : Super(L"RigidBody", ComponentType::RigidBody) {}
		virtual ~RigidBodyComponent() override {}

		virtual void Init() override;
		virtual void Tick(const float& DeltaTime) override;
		virtual void Render(IRenderer* r) override;

		void SetGround(bool ground) { _bGround = ground; }
		bool IsGround() const { return _bGround; }
		void SetMass(float mass) { _mass = mass; }
		void SetFriction(float friction) { _friction = friction; }
		void AddForce(Vector3 force) { _force = force; }
		void SetVelocity(Vector3 velocity) { _velocity = velocity; }
		Vector3 GetVelocity() const { return _velocity; }
		void SetVelocityLimit(Vector2 limit) { _velocityLimit = limit; }
		void SetGravity(Vector3 gravity) { _gravity = gravity; _gravityDir = _gravity.GetNormalize(); }

		void CalculateVelocity(const float& DeltaTime);
		void GravityForce(const float& DeltaTime);
		void LimitVelocity();
		void FrictionForce(const float& DeltaTime);
	private:
		bool _bGround = false;								// 지면에 닿아있는지
		float _mass = 1.f;									// 질량
		float _friction = 10.f;								// 마찰력 (속도의 반대방향)

		Vector3 _force = Vector3::Zero;						// 힘
		Vector3 _acceleration = Vector3::Zero;				// 가속도 (force / mass)
		Vector3 _velocity = Vector3::Zero;					// 속도 + 방향 += 가속도 * 시간
		Vector2 _velocityLimit = Vector2(200.f, 1000.f);	// 속도 제한 (이 이상 속도가 올라가지 않도록), Vector2(속도 제한, 중력 방향 속도 제한)
		Vector3 _gravity = Vector3(0.f, 800.f, 0.f);		// 중력 (아래로 향하는 힘), 3차원에서 중력의 방향은 무조건 밑이 아닐수도 있음
		Vector3 _gravityDir = Vector3::Zero;				// 매번 중력 방향을 계산하기 보다는 저장해서 사용
    };
}