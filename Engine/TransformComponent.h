#pragma once
#include "Component.h"

namespace JE {
	class TransformComponent : public Component
	{
		using Super = Component;
	public:
		TransformComponent(const std::wstring& name) : Super(name, ComponentType::Transform) {}
		virtual ~TransformComponent() override {}

		// Position
		void SetPos(const Vector3& pos) { _pos = pos; }
		void AddPos(const Vector3& deltaPos) { _pos += deltaPos; }
		Vector3 GetPos() const { return _pos; }

		// Rotation (회전하면 각 axis의 축도 회전됨) - 내부 시스템은 Quaternion을 사용해 회전
		// 오일러 각을 사용하는 회전 (Yaw = y축 회전 (좌우), Roll = Z축 회전(회전), Pitch = X축 회전(위아래))
		void SetRotation(const Rotator& rotator) { _rotation = Quaternion(rotator); }
		// 회전 변환행렬
		void SetRotation(const Matrix3x3& mat) { _rotation = Quaternion(mat); }
		// 사원수를 이용한 회전
		void SetRotation(const Quaternion& q) { _rotation = q; }

		// 추가 회전
		void AddYaw(const float& deltaDegree);
		void AddRoll(const float& deltaDegree);
		void AddPitch(const float& deltaDegree);
		
		Quaternion GetRotation() const { return _rotation; }

		// Scale
		void SetScale(const Vector3& scale) { _scale = scale;	}
		void AddScale(const Vector3& deltaScale) { _scale += deltaScale; }
		Vector3 GetScale() const { return _scale; }

		// Local Axes
		// 회전 사원수로 로컬축 벡터를 회전
		const Vector3 GetLocalX() const { return _rotation * _right; }
		const Vector3 GetLocalY() const { return _rotation * _up; }
		const Vector3 GetLocalZ() const { return _rotation * _forward; }

		// 가장 기본적인 크기 -> 회전 -> 이동변환
		Matrix4x4 GetTransformMatrix() const;
		// 이동 변환 행렬
		Matrix4x4 GetTranslationMatrix() const;
		// 회전 변환 행렬
		Matrix4x4 GetRotationMatrix() const;
		// 크기 변환 행렬
		Matrix4x4 GetScaleMatrix() const;

		// 이동 역변환 행렬
		Matrix4x4 GetInvTranslationMatrix() const;
		// 회전 역변환 행렬
		Matrix4x4 GetInvRotationMatrix() const;
		// 크기 역변환 행렬
		Matrix4x4 GetInvScaleMatrix() const;

		// 로컬 축 지정
		void SetLocalAxes(const Vector3& right, const Vector3& up, const Vector3& forward) {
			_right = right;
			_up = up;
			_forward = forward;
		}
	private:
		// 물체를 기준으로 설정된 방향 정보인 Local Axis를 회전값에 따라 회전하므로 회전된 로컬축 갱신
		// 오일러 각을 사용해 회전할때 사용 (사원수를 사용하면 사용하지 않음)
		void updateAxes();

	private:
		Vector3 _pos = Vector3::Zero;
		Quaternion _rotation;
		Vector3 _scale = Vector3::One;

		// Local 축
		Vector3 _right = Vector3::UnitX;
		Vector3 _up = Vector3::UnitY;
		Vector3 _forward = Vector3::UnitZ;
	};
}

