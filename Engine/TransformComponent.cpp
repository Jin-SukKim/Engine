#include "pch.h"
#include "TransformComponent.h"

namespace JE {
	void TransformComponent::AddYaw(const float& deltaDegree)
	{
		Rotator r = _rotation.ToRotator(); // 사원수 -> 오일러 각으로 변환
		r.Yaw += deltaDegree; // 회전각 더하기
		r.Clamp(); // 오일러 회전각을 사용하기 편하게 재구성 [0, 360] 범위
		_rotation = Quaternion(r); // 오일러 각 -> 사원수로 변환
	}
	void TransformComponent::AddRoll(const float& deltaDegree)
	{
		Rotator r = _rotation.ToRotator();
		r.Roll += deltaDegree;
		r.Clamp();
		_rotation = Quaternion(r);
	}
	void TransformComponent::AddPitch(const float& deltaDegree)
	{
		Rotator r = _rotation.ToRotator(); 
		r.Pitch += deltaDegree; 
		r.Clamp();
		_rotation = Quaternion(r);
	}
	Matrix4x4 TransformComponent::GetTransformMatrix() const
	{
		/*
			추가적인 공간과 계산이 필요 (각 행렬을 생성하는 공간 + 각 행렬끼리 매번 계산)
			return GetScaleMatrix() * GetRotationMatrix() * GetTranslation Matrix();

			최종 행렬 (위의 return값과 계산 결과는 동일)

			이 방식을 추천하는데 TransformComponent가 가지고 있는 값들만 가지고 
			계산을 진행하기에 최소한의 공간과 계산으로 행렬을 생성

			Vector만 생성 + 가지고 있는 값으로만 계산
		*/
		return Matrix4x4(
			// GetLocal() 함수로 회전 사원수에 의해 회전된 로컬축을 가져오고 scale과 pos를 적용
			Vector4(GetLocalX() * _scale.X, 0.f),
			Vector4(GetLocalY() * _scale.Y, 0.f),
			Vector4(GetLocalZ() * _scale.Z, 0.f),
			Vector4(_pos, 1.f)
		);
	}
	Matrix4x4 TransformComponent::GetTranslationMatrix() const
	{
		return Matrix4x4::TranslationTransform(_pos);
	}
	Matrix4x4 TransformComponent::GetRotationMatrix() const
	{
		// 사원수를 사용해 계산한 로컬 축을 활용해 회전 행렬 계산
		return Matrix4x4::RotationTransform(GetLocalX(), GetLocalY(), GetLocalZ());
	}
	Matrix4x4 TransformComponent::GetScaleMatrix() const
	{
		return Matrix4x4::ScaleTransform(_scale);
	}
	Matrix4x4 TransformComponent::GetInvTranslationMatrix() const
	{
		return Matrix4x4::InverseTranslationTransform(GetTranslationMatrix());
	}
	Matrix4x4 TransformComponent::GetInvRotationMatrix() const
	{
		return Matrix4x4::InversRotationTransform(GetRotationMatrix());
	}
	Matrix4x4 TransformComponent::GetInvScaleMatrix() const
	{
		return Matrix4x4::InverseScaleTransform(GetScaleMatrix());
	}
	void TransformComponent::updateAxes()
	{
		// 오일러 각만을 사용해 회전했을 때 각도를 더하고 회전축을 갱신
		//_rotation.Clamp();
		//_rotation.SetLocalAxes(_right, _up, _forward);
	}
}