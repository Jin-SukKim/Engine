#pragma once
#include "Component.h"
#include "TransformComponent.h"

namespace JE {
	class Actor;

	class CameraComponent : public Component
	{
		using Super = Component;
	public:
		CameraComponent(const std::wstring& name);
		virtual ~CameraComponent() override {}

		virtual void Tick(const float& DeltaTime) override;

		// 절두체 생성
		void CreateFrustum();
		// 모델링 행렬을 받아 모델링 행렬(로컬) * 뷰 행렬 * 원근 투영 행렬을 사용해
		// 로컬 공간의 좌표를 받아 절두체 컬링을 수행할 수 있는 절두체를 생성
		void CreateFrustum(const Matrix4x4& mat);
		// 뷰 공간에서의 절두체 컬링을 위한 절두체
		const Frustum& GetFrustum() const { return _viewFrustum; }

		// 카메라의 Viewport 크기
		const ScreenPoint& GetVeiwportSize() const { return _viewportSize; }
		void SetViewportSize(const ScreenPoint& size) { _viewportSize = size; }

		/*
			카메라가 보는 방향에 따라 외적을 사용해 회전된 로컬 축을 설정해 회전 행렬에 사용
			보통 월드의 Up벡터인 y축을 사용하지만 카메라가 뒤집어진 경우가 있을 수 있으니 up 벡터를 Parameter로 받음
			카메라가 뒤집어진 경우 up 벡터를 -Vector3::UnitY를 받아야 함

			설정한 TargetPos에 시점을 두도록 카메라를 회전
		*/
		void SetLookAt(const Vector3& targetPos, const Vector3& up = Vector3::UnitY);
		void SetLookAt(Object* object, const Vector3& up = Vector3::UnitY);

		// 카메라 화각 설정
		void SetFOV(float fov) { _fov = fov; }
		const float GetFOV() const { return _fov; }

		const Vector3 GetViewDir() const;

		// 뷰 행렬 (이동 -> 회전의 역변환과 같음)
		Matrix4x4 GetViewMatrix() const;
		// 뷰 행렬에 회전만 적용해서 가져오기
		Matrix4x4 GetViewRotationMatrix() const;
		/*
			원근 투영 행렬 (뷰 공간의 점을 투영 평면(NDC) 위의 점으로 대응시키기 위해 클릭 좌표계로 변환하는 행렬)
			뷰 좌표(뷰 행렬) -> 클릭 좌표(원근 투영 행렬) -> NDC 좌표(각 정점의 세번째 값인 Z값으로 나누기)
			클립 좌표의 W 값은 결국 뷰 공간의 깊이 값을 의미하고 이를 나눠주면 Z 값이 NDC 좌표의 깊이 값이 됨
		*/
		Matrix4x4 GetPerspectiveMatrix() const;
		// 뷰 x 투영 행렬
		Matrix4x4 GetViewPerspectiveMatrix() const;

		// 직교 투영 행렬
		Matrix4x4 GetOrthographicMatrix() const;

		// 카메라의 Transform
		TransformComponent* GetTransform() const { return _cameraTransform.get(); }
		void GetViewAxes(Vector3& outViewX, Vector3& outViewY, Vector3& outViewZ) const;

		void SetNearZ(const float& nearZ) { _nearZ = nearZ;	}
		float GetNearZ() const { return _nearZ; }

		void SetFarZ(const float& farZ) { _farZ = farZ;	}
		float GetFarZ() const { return _farZ; }

		// Actor에 카메라 부착
		void AttachCamera(Actor* actor) { _attached = actor; }
		void DettachCamera() { _attached = nullptr; }

		void SetMinDistance(float distance) { _minDistance = distance; }
		void SetLerpSpeed(float speed) { _lerpSpeed = speed; }
	private:
		ScreenPoint _viewportSize;
		/*
			카메라에 보이는 범위 (화각) - NDC는 언제나 가운데 (0, 0)을 원점으로 좌우상하 길이 1의 값을 가짐
			카메라의 화각이 변할 때 투영 편면에 대응하는 NDC가 일정한 값을 가져야하므로 초점거리가 변함
			tan(fov/2) = 1 / d, d = 초점거리, d = 1 / (tan(fov / 2))
		*/ 
		float _fov = 60.f;
		float _nearZ = 5.5f;
		float _farZ = 5000.f;

		Frustum _viewFrustum;

		std::unique_ptr<TransformComponent> _cameraTransform;

		// owner를 한 템포 늦게 따라가는 시각적 효과인 Lagging 효과를 위한 거리
		float _minDistance = 0.f;
		float _lerpSpeed = 1.f; // 카메라가 따라가는 속도

		Actor* _attached = nullptr;
	};
}
