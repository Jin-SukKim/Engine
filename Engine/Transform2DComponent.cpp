#include "pch.h"
#include "Transform2DComponent.h"

namespace JE {
	Matrix3x3 Transform2DComponent::GetTransformMatrix() const
	{
		// ũ�� -> ȸ�� -> �̵� ��ȯ
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
		// ȸ������ ���� sin, cos ��
		float sin, cos;
		MathUtils::GetSinCos(sin, cos, _rotation);

		_right = Vector2(cos, sin); // x�� ȸ��
		_up = Vector2(-sin, cos); // y�� ȸ��
	}
}