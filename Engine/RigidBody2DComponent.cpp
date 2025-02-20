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

		// 현재 속도 계산
		CalculateVelocity(DeltaTime);

		// 중력 작용 (땅 위에 있을 때와 공중에 있을 때 다르게 작용)
		GravityForce(DeltaTime);

		// 최대 속도 제한 (중력 방향과 나머지 방향을 각각 조절, 즉 낙하는 속도 제한보다 더 빨라질 수 있음)
		LimitVelocity();

		// 마찰력 작용
		FrictionForce(DeltaTime);

		SpriteActor* owner = dynamic_cast<SpriteActor*>(this->GetOwner());
		if (!owner)
			return;

		Transform2DComponent* tr = owner->GetComponent<Transform2DComponent>();
		if (!tr)
			return;

		// 현재 속도만큼 속도 방향으로 이동
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
		// 가속도 계산, accelerate = force / mass, (force = mass x accelerate)
		_acceleration = _force / _mass;

		// 힘은 순간적으로 작용 (밀기는 지속적으로 힘을 가하는 것)
		_force = Vector2::Zero;

		// 속도 계산, velocity += accelerate * deltaTime
		_velocity += _acceleration * DeltaTime;
	}

	void RigidBody2DComponent::GravityForce(const float& DeltaTime)
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

	void RigidBody2DComponent::LimitVelocity()
	{
		// 최대 속도 제한 (중력 방향과 나머지 방향을 각각 조절, 즉 낙하는 속도 제한보다 더 빨라질 수 있음)

		// 현재 속도의 중력 방향 성분 계산
		float dot = _gravityDir.Dot(_velocity);

		Vector2 gravityVelocity = _gravityDir * dot; // 중력 방향으로 투영한 것과 같음

		// 좌우 방향 속도 계산
		Vector2 sideVelocity = _velocity - gravityVelocity; // 현재 속도 - 중력

		// 중력 방향 속도 제한
		float gravityLimit = _velocityLimit.Y; // 중력 방향 속도 제한
		float gravitySpeed = gravityVelocity.Size();
		if (gravityLimit < gravitySpeed)
		{
			// ex) 현재 중력 방향 속도가 100이고 제한이 50이라면 비율 계산 현재 속도에 비율을 곱하면
			// 속도가 정확히 제한값으로 줄어듬. 속도가 100이었다면 0.5를 곱해서 50이 됨 
			// (나눗셈으로 방향도 유지, 빼기면 방향이 바뀔 수 있음)
			gravityVelocity = gravityVelocity * (_velocityLimit.Y / gravitySpeed);
		}

		// 좌우 방향 속도 제한
		float horizontalLimit = _velocityLimit.X; // 나머지 속도 제한
		float sideSpeed = sideVelocity.Size();
		if (horizontalLimit < sideSpeed)
		{
			sideVelocity = sideVelocity * (_velocityLimit.X / sideSpeed);
		}

		// 최대 속도 제한된 현재 속도
		_velocity = gravityVelocity + sideVelocity;
	}

	void RigidBody2DComponent::FrictionForce(const float& DeltaTime)
	{
		// 속도가 0이 아니라면 마찰력 작용
		if (!(_velocity == Vector2::Zero)) {
			// 속도에 반대방향으로 마찰력 작용
			Vector2 frictionDir = (-_velocity).GetNormalize();
			// 현재 마찰력 = 마찰력 방향 * 마찰력 * 무게 * 시간
			Vector2 currentFriction = frictionDir * _friction * _mass * DeltaTime;

			// 마찰력으로 인한 속도 감소량이 현재 속도보다 크면
			if (_velocity.SizeSquared() <= currentFriction.SizeSquared())
				// 속도가 0이 되면서 정지
				_velocity = Vector2::Zero;
			else
				_velocity += currentFriction;
		}
	}
}