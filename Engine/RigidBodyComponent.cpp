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

		// 현재 속도 계산
		CalculateVelocity(DeltaTime);

		// 중력 작용 (땅 위에 있을 때와 공중에 있을 때 다르게 작용)
		GravityForce(DeltaTime);

		// 최대 속도 제한 (중력 방향과 나머지 방향을 각각 조절, 즉 낙하는 속도 제한보다 더 빨라질 수 있음)
		LimitVelocity();

		// 마찰력 작용
		FrictionForce(DeltaTime);

		Actor* owner = dynamic_cast<Actor*>(this->GetOwner());
		if (!owner)
			return;

		TransformComponent* tr = owner->GetComponent<TransformComponent>();
		if (!tr)
			return;

		// 현재 속도만큼 속도 방향으로 이동
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
		// 가속도 계산, accelerate = force / mass, (force = mass x accelerate)
		_acceleration = _force / _mass;

		// 힘은 순간적으로 작용 (밀기는 지속적으로 힘을 가하는 것)
		_force = Vector3::Zero;

		// 속도 계산, velocity += accelerate * deltaTime
		_velocity += _acceleration * DeltaTime;
	}
	
	void RigidBodyComponent::GravityForce(const float& DeltaTime)
	{
		// 중력과 속도/방향의 관계 
		// (게임에선 항상 중력이 아래로 작동하지 않고 땅이나 벽에 붙어있는 경우 다른 방향으로 작동하기도 함)
		float dot = _gravityDir.Dot(_velocity);

		// 땅 위에 있을때
		if (_bGround)
			// 중력의 영향으로 느려지는 속도 혹은 경사가 있을 때의 영향
			_velocity -= _gravity * dot * DeltaTime;
		// 공중에 있을때
		else
			_velocity -= _gravity * DeltaTime;
	}
	
	void RigidBodyComponent::LimitVelocity()
	{
		// 최대 속도 제한 (중력 방향과 나머지 방향을 각각 조절, 즉 낙하는 속도 제한보다 더 빨라질 수 있음)

		// 1. 속도를 중력 방향 성분과 수직 성분으로 분해
		float dot = _gravityDir.Dot(_velocity);
		Vector3 gravityVelocity = _gravityDir * dot;  // 현재 속도 중에서 중력 방향의 속도

		// 중력 방향 속도를 빼서 중력 방향을 제외한 속도를 계산
		Vector3 planeVelocity = _velocity - gravityVelocity;  // 중력에 수직인 평면상의 속도

		// 2. 중력 방향 속도 제한
		float gravityLimit = _velocityLimit.Y; // 중력 방향 속도 제한
		float gravitySpeed = gravityVelocity.Size();
		if (gravityLimit < gravitySpeed)
		{
			// ex) 현재 중력 방향 속도가 100이고 제한이 50이라면 비율 계산 현재 속도에 비율을 곱하면
			// 속도가 정확히 제한값으로 줄어듬. 속도가 100이었다면 0.5를 곱해서 50이 됨 
			// (나눗셈으로 방향도 유지, 빼기면 방향이 바뀔 수 있음)
			gravityVelocity = gravityVelocity * (gravityLimit / gravitySpeed);
		}

		// 3. 수평 방향(중력에 수직인 평면상) 속도 제한
		float horizontalLimit = _velocityLimit.X; // 나머지 속도 제한
		float planeSpeed = planeVelocity.Size();
		if (horizontalLimit < planeSpeed)
		{
			planeVelocity = planeVelocity * (horizontalLimit / planeSpeed);
		}

		// 4. 제한된 속도 성분들을 다시 합성
		_velocity = gravityVelocity + planeVelocity;
	}
	
	void RigidBodyComponent::FrictionForce(const float& DeltaTime)
	{
		// 속도가 0이 아니라면 마찰력 작용
		if (!(_velocity == Vector3::Zero)) {
			// 속도에 반대방향으로 마찰력 작용
			Vector3 frictionDir = (-_velocity).GetNormalize();
			// 현재 마찰력 = 마찰력 방향 * 마찰력 * 무게 * 시간
			Vector3 currentFriction = frictionDir * _friction * _mass * DeltaTime;

			// 마찰력으로 인한 속도 감소량이 현재 속도보다 크면
			if (_velocity.SizeSquared() <= currentFriction.SizeSquared())
				// 속도가 0이 되면서 정지
				_velocity = Vector3::Zero;
			else
				_velocity += currentFriction;
		}
	}
}