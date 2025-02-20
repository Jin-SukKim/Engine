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
		// ���������� �������� ��
		void AddForce(Vector2 force) { _force = force; }
		void SetVelocity(Vector2 velocity) { _velocity = velocity; }
		Vector2 GetVelocity() const { return _velocity; }
		// Vector2(�ӵ� ����, �߷� ���� �ӵ� ����)
		void SetVelocityLimit(Vector2 limit) { _velocityLimit = limit; }
		// Vector2(0, �߷� ����� �߷�)
		void SetGravity(Vector2 gravity) { _gravity = gravity; _gravityDir = _gravity.GetNormalize(); }

		void CalculateVelocity(const float& DeltaTime);
		void GravityForce(const float& DeltaTime);
		void LimitVelocity();
		void FrictionForce(const float& DeltaTime);
	private:
		bool _bGround = false;								// ���鿡 ����ִ���
		float _mass = 1.f;									// ����
		float _friction = 10.f;								// ������ (�ӵ��� �ݴ� ����)

		Vector2 _force = Vector2::Zero;						// ��
		Vector2 _acceleration = Vector2::Zero;				// ���ӵ� (force / mass)
		Vector2 _velocity = Vector2::Zero;					// �ӵ� + ���� = ���ӵ� * �ð�
		Vector2 _velocityLimit = Vector2(200.f, 1000.f);	// �ӵ� ���� (�� �̻� �ӵ��� �ö��� �ʵ���), Vector2(�ӵ� ����, �߷� ���� �ӵ� ����)
		Vector2 _gravity = Vector2(0.f, 800.f);				// �߷� (�Ʒ��� ���ϴ� ��), Vecotr2(0, �߷� ����� �߷�)
		Vector2 _gravityDir = Vector2::Zero;				// �Ź� �߷� ������ ����ϱ� ���ٴ� �����ؼ� ���
	};
}
