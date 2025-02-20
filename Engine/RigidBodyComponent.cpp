#include "pch.h"
#include "RigidBodyComponent.h"
#include "Actor.h"
#include "TransformComponent.h"

namespace JE {
	void RigidBodyComponent::Init()
	{
		Super::Init();
	}
	void RigidBodyComponent::Tick(const float& DeltaTime)
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

		Actor* owner = dynamic_cast<Actor*>(this->GetOwner());
		if (!owner)
			return;

		TransformComponent* tr = owner->GetComponent<TransformComponent>();
		if (!tr)
			return;

		// ���� �ӵ���ŭ �ӵ� �������� �̵�
		Vector3 pos = tr->GetPos();
		pos += _velocity * DeltaTime;
		tr->SetPos(pos);
	}
	void RigidBodyComponent::Render(IRenderer* r)
	{
		Super::Render(r);
	}

	void RigidBodyComponent::CalculateVelocity(const float& DeltaTime)
	{
		// ���ӵ� ���, accelerate = force / mass, (force = mass x accelerate)
		_acceleration = _force / _mass;

		// ���� ���������� �ۿ� (�б�� ���������� ���� ���ϴ� ��)
		_force = Vector3::Zero;

		// �ӵ� ���, velocity += accelerate * deltaTime
		_velocity += _acceleration * DeltaTime;
	}
	
	void RigidBodyComponent::GravityForce(const float& DeltaTime)
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
	
	void RigidBodyComponent::LimitVelocity()
	{
		// �ִ� �ӵ� ���� (�߷� ����� ������ ������ ���� ����, �� ���ϴ� �ӵ� ���Ѻ��� �� ������ �� ����)

		// 1. �ӵ��� �߷� ���� ���а� ���� �������� ����
		float dot = _gravityDir.Dot(_velocity);
		Vector3 gravityVelocity = _gravityDir * dot;  // ���� �ӵ� �߿��� �߷� ������ �ӵ�

		// �߷� ���� �ӵ��� ���� �߷� ������ ������ �ӵ��� ���
		Vector3 planeVelocity = _velocity - gravityVelocity;  // �߷¿� ������ ������ �ӵ�

		// 2. �߷� ���� �ӵ� ����
		float gravityLimit = _velocityLimit.Y; // �߷� ���� �ӵ� ����
		float gravitySpeed = gravityVelocity.Size();
		if (gravityLimit < gravitySpeed)
		{
			// ex) ���� �߷� ���� �ӵ��� 100�̰� ������ 50�̶�� ���� ��� ���� �ӵ��� ������ ���ϸ�
			// �ӵ��� ��Ȯ�� ���Ѱ����� �پ��. �ӵ��� 100�̾��ٸ� 0.5�� ���ؼ� 50�� �� 
			// (���������� ���⵵ ����, ����� ������ �ٲ� �� ����)
			gravityVelocity = gravityVelocity * (gravityLimit / gravitySpeed);
		}

		// 3. ���� ����(�߷¿� ������ ����) �ӵ� ����
		float horizontalLimit = _velocityLimit.X; // ������ �ӵ� ����
		float planeSpeed = planeVelocity.Size();
		if (horizontalLimit < planeSpeed)
		{
			planeVelocity = planeVelocity * (horizontalLimit / planeSpeed);
		}

		// 4. ���ѵ� �ӵ� ���е��� �ٽ� �ռ�
		_velocity = gravityVelocity + planeVelocity;
	}
	
	void RigidBodyComponent::FrictionForce(const float& DeltaTime)
	{
		// �ӵ��� 0�� �ƴ϶�� ������ �ۿ�
		if (!(_velocity == Vector3::Zero)) {
			// �ӵ��� �ݴ�������� ������ �ۿ�
			Vector3 frictionDir = (-_velocity).GetNormalize();
			// ���� ������ = ������ ���� * ������ * ���� * �ð�
			Vector3 currentFriction = frictionDir * _friction * _mass * DeltaTime;

			// ���������� ���� �ӵ� ���ҷ��� ���� �ӵ����� ũ��
			if (_velocity.SizeSquared() <= currentFriction.SizeSquared())
				// �ӵ��� 0�� �Ǹ鼭 ����
				_velocity = Vector3::Zero;
			else
				_velocity += currentFriction;
		}
	}
}