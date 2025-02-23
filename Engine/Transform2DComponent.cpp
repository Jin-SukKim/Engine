#include "pch.h"
#include "Transform2DComponent.h"

namespace JE {
	Matrix3x3 Transform2DComponent::GetTransformMatrix() const
	{
		// 크기 -> 회전 -> 이동 변환
		return GetScaleMatrix() * GetRotationMatrix() * GetTranslationMatrix();
	}

	Matrix3x3 Transform2DComponent::GetTranslationMatrix() const
	{
		return Matrix3x3::TranslationTransform(_pos);
	}

	Matrix3x3 Transform2DComponent::GetRotationMatrix() const
	{
		return Matrix3x3::RotationTransform(_rotation);
	}

	Matrix3x3 Transform2DComponent::GetScaleMatrix() const
	{
		return Matrix3x3::ScaleTransform(_scale);
	}

	void Transform2DComponent::updateAxes()
	{
		// 회전각에 따른 sin, cos 값
		float sin, cos;
		MathUtils::GetSinCos(sin, cos, _rotation);

		_right = Vector2(cos, sin); // x축 회전
		_up = Vector2(-sin, cos); // y축 회전
	}
}