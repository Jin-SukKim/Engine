#include "pch.h"
#include "RigidBody2DComponent.h"
#include "SpriteActor.h"
#include "Transform2DComponent.h"

namespace JE {
	void RigidBody2DComponent::Init()
	{
		Super::Init();
	}

	void RigidBody2DComponent::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);

		// ���� �ӵ� ���
		CalculateVelocity(DeltaTime);

		// �߷� �ۿ� (�� ���� ���� ���� ���߿� ���� �� �ٸ��� �ۿ�)
		GravityForce(DeltaTime);

		// �ִ� �ӵ� ���� (�߷� ����� ������ ������ ���� ����, �� ���ϴ� �ӵ� ���Ѻ��� �� ������ �� ����)
		LimitVelocity();

		// ������ �ۿ�
		FrictionForce(DeltaTime);

		SpriteActor* owner = dynamic_cast<SpriteActor*>(this->GetOwner());
		if (!owner)
			return;

		Transform2DComponent* tr = owner->GetComponent<Transform2DComponent>();
		if (!tr)
			return;

		// ���� �ӵ���ŭ �ӵ� �������� �̵�
		Vector2 pos = tr->GetPos();
		pos += _velocity * DeltaTime;
		tr->SetPos(pos);
	}

	void RigidBody2DComponent::Render(IRenderer* r)
	{
		Super::Render(r);
	}

	void RigidBody2DComponent::CalculateVelocity(const float& DeltaTime)
	{
		// ���ӵ� ���, accelerate = force / mass, (force = mass x accelerate)
		_acceleration = _force / _mass;

		// ���� ���������� �ۿ� (�б�� ���������� ���� ���ϴ� ��)
		_force = Vector2::Zero;

		// �ӵ� ���, velocity += accelerate * deltaTime
		_velocity += _acceleration * DeltaTime;
	}

	void RigidBody2DComponent::GravityForce(const float& DeltaTime)
	{
		// �߷°� �ӵ�/������ ���� 
		// (���ӿ��� �׻� �߷��� �Ʒ��� �۵����� �ʰ� ���̳� ���� �پ��ִ� ��� �ٸ� �������� �۵��ϱ⵵ ��)
		float dot = _gravityDir.Dot(_velocity);

		// �� ���� ������
		if (_bGround)
			// �߷��� �������� �������� �ӵ� Ȥ�� ��簡 ���� ���� ����
			_velocity -= _gravity * dot * DeltaTime;
		// ���߿� ������
		else
			_velocity -= _gravity * DeltaTime;
	}

	void RigidBody2DComponent::LimitVelocity()
	{
		// �ִ� �ӵ� ���� (�߷� ����� ������ ������ ���� ����, �� ���ϴ� �ӵ� ���Ѻ��� �� ������ �� ����)

		// ���� �ӵ��� �߷� ���� ���� ���
		float dot = _gravityDir.Dot(_velocity);

		Vector2 gravityVelocity = _gravityDir * dot; // �߷� �������� ������ �Ͱ� ����

		// �¿� ���� �ӵ� ���
		Vector2 sideVelocity = _velocity - gravityVelocity; // ���� �ӵ� - �߷�

		// �߷� ���� �ӵ� ����
		float gravityLimit = _velocityLimit.Y; // �߷� ���� �ӵ� ����
		float gravitySpeed = gravityVelocity.Size();
		if (gravityLimit < gravitySpeed)
		{
			// ex) ���� �߷� ���� �ӵ��� 100�̰� ������ 50�̶�� ���� ��� ���� �ӵ��� ������ ���ϸ�
			// �ӵ��� ��Ȯ�� ���Ѱ����� �پ��. �ӵ��� 100�̾��ٸ� 0.5�� ���ؼ� 50�� �� 
			// (���������� ���⵵ ����, ����� ������ �ٲ� �� ����)
			gravityVelocity = gravityVelocity * (_velocityLimit.Y / gravitySpeed);
		}

		// �¿� ���� �ӵ� ����
		float horizontalLimit = _velocityLimit.X; // ������ �ӵ� ����
		float sideSpeed = sideVelocity.Size();
		if (horizontalLimit < sideSpeed)
		{
			sideVelocity = sideVelocity * (_velocityLimit.X / sideSpeed);
		}

		// �ִ� �ӵ� ���ѵ� ���� �ӵ�
		_velocity = gravityVelocity + sideVelocity;
	}

	void RigidBody2DComponent::FrictionForce(const float& DeltaTime)
	{
		// �ӵ��� 0�� �ƴ϶�� ������ �ۿ�
		if (!(_velocity == Vector2::Zero)) {
			// �ӵ��� �ݴ�������� ������ �ۿ�
			Vector2 frictionDir = (-_velocity).GetNormalize();
			// ���� ������ = ������ ���� * ������ * ���� * �ð�
			Vector2 currentFriction = frictionDir * _friction * _mass * DeltaTime;

			// ���������� ���� �ӵ� ���ҷ��� ���� �ӵ����� ũ��
			if (_velocity.SizeSquared() <= currentFriction.SizeSquared())
				// �ӵ��� 0�� �Ǹ鼭 ����
				_velocity = Vector2::Zero;
			else
				_velocity += currentFriction;
		}
	}
}