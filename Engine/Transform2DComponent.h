#pragma once
#include "Component.h"

namespace JE {
	class Transform2DComponent : public Component
	{
	public:
		Transform2DComponent(const std::wstring& name) : Component(name, ComponentType::Transform) {}
		virtual ~Transform2DComponent() override {}

		// Translation
		void SetPos(const Vector2& pos) { _pos = pos; }
		void AddPos(const Vector2& deltaPos) { _pos += deltaPos; }
		Vector2 GetPos() const { return _pos; }

		// Rotation (ȸ���ϸ� ������ axis�� ȸ���ǹǷ� ����)
		void SetRotation(float degree) { _rotation = degree; updateAxes(); }
		void AddRotation(float deltaDegree) { _rotation += deltaDegree; updateAxes(); }
		// ȸ����
		float GetRotation() const { return _rotation; }

		// Scale
		void SetScale(const float& scale) { _scale = { scale, scale }; }
		void SetScale(const Vector2& scale) { _scale = scale; }
		void ChangeScale(const Vector2& scale) { _scale *= scale; }
		void ChangeScale(const float& scale) { _scale *= scale; }
		void AddScale(const float& deltaScale) { _scale *= deltaScale; }
		void AddScale(const Vector2& deltaScale) { _scale *= deltaScale; }
		Vector2 GetScale() const { return _scale; }

		// Local Axis
		const Vector2& GetLocalX() const { return _right; }
		const Vector2& GetLocalY() const { return _up; }

		// ���� �⺻���� ũ�� -> ȸ�� -> �̵� ��ȯ
		Matrix3x3 GetTransformMatrix() const;

		// �̵� ��ȯ ���
		Matrix3x3 GetTranslationMatrix() const;

		// ȸ�� ��ȯ ���
		Matrix3x3 GetRotationMatrix() const;
		
		// ũ�� ��ȯ ���
		Matrix3x3 GetScaleMatrix() const;

	private:
		// ��ü�� �������� ������ ���� ������ Local Axis�� ȸ������ ���� ȸ��
		void updateAxes();

	private:
		Vector2 _pos = Vector2::Zero;
		Vector2 _scale = Vector2::One;
		float _rotation = 0.f;

		Vector2 _right = Vector2::UnitX;
		Vector2 _up = Vector2::UnitY;
	};
}

