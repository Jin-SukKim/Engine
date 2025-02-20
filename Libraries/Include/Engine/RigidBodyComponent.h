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
		bool _bGround = false;								// ���鿡 ����ִ���
		float _mass = 1.f;									// ����
		float _friction = 10.f;								// ������ (�ӵ��� �ݴ����)

		Vector3 _force = Vector3::Zero;						// ��
		Vector3 _acceleration = Vector3::Zero;				// ���ӵ� (force / mass)
		Vector3 _velocity = Vector3::Zero;					// �ӵ� + ���� += ���ӵ� * �ð�
		Vector2 _velocityLimit = Vector2(200.f, 1000.f);	// �ӵ� ���� (�� �̻� �ӵ��� �ö��� �ʵ���), Vector2(�ӵ� ����, �߷� ���� �ӵ� ����)
		Vector3 _gravity = Vector3(0.f, 800.f, 0.f);		// �߷� (�Ʒ��� ���ϴ� ��), 3�������� �߷��� ������ ������ ���� �ƴҼ��� ����
		Vector3 _gravityDir = Vector3::Zero;				// �Ź� �߷� ������ ����ϱ� ���ٴ� �����ؼ� ���
    };
}