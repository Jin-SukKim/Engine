#include "pch.h"
#include "CameraComponent.h"
#include "Actor.h"

namespace JE {
	CameraComponent::CameraComponent(const std::wstring& name) : Super(name, ComponentType::Camera)
	{
		_cameraTransform = std::make_unique<TransformComponent>(L"CameraTransform");
	}

	void CameraComponent::Tick(const float& DeltaTime)
	{
		if (_attached) {
			TransformComponent* tr = _attached->GetComponent<TransformComponent>();
			if (tr) {
				Vector3 ownerPos = tr->GetPos();
				Vector3 cameraPos = _cameraTransform->GetPos();

				// 플레이어를 한 템포 늦게 따라가는 시각적 효과인 Lagging 효과
				// minDistance == 0이면 Lagging 효과없이 바로 따라가기
				if (_minDistance == 0.f || (ownerPos - cameraPos).SizeSquared() < _minDistance * _minDistance)
					_cameraTransform->SetPos(ownerPos);
				else {
					// 현재 프레임에서 쫓아갈 비율 계산
					float ratio = std::clamp(_lerpSpeed * DeltaTime, 0.f, 1.f);
					// 카메라의 현재 위치에서 플레이어의 현재 위치까지 이동하는 벡터를 구하고 ratio를 곱해 이동할 위치 계산
					Vector3 newPos = MathUtils::Lerp(cameraPos, ownerPos, ratio);
					_cameraTransform->SetPos(newPos);
				}
			}
		}
	}

	void CameraComponent::CreateFrustum()
	{
		// 평면의 방정식을 사용해 절두체 구축
		/*
		float halfFOV = GetFOV() * 0.5f;
		float sin = 0.f, cos = 0.f;
		MathUtils::GetSinCos(sin, cos, halfFOV);

		// 가로 세로가 균일하다고 생각하고 만들었기에 가로, 세로 비율이 맞지 않음
		std::array<Plane, 6> frustumPlanes = {
			// 상하좌우 평면은 무한히 넓어졌을 때 원점과의 거리가 0이므로 d를 0으로 설정
			// 상하좌우 평면은 카메라에 모이도록 회전되어 있음
			Plane(Vector3(cos, 0.f, -sin), 0.f), // 좌측 (-Y)
			Plane(Vector3(-cos, 0.f, -sin), 0.f), // 우측 (+Y)
			Plane(Vector3(0.f, -cos, -sin), 0.f), // 하단 (-X)
			Plane(Vector3(0.f, cos, -sin), 0.f), // 상단 (+X)
			// 근평면, 원평면은 원점과의 거리가 nearZ, farZ이고 근평면의 법선 벡터는 -normal, 원평면은 + normal
			// 근평면은 평면의 앞면이 원점을 향하므로 d가 양수,
			// 원평면은 원점과 멀어지는 방향이기에 d가 음수
			Plane(-Vector3::UnitZ, _nearZ), // 근평면(-Z)
			Plane(Vector3::UnitZ, -_farZ) // 원평면(+Z)
		};
		*/
		
		// 원근 투영 행렬을 활용한 더 간편하고 빠른 절두체 컬링을 위한 절두체 생성
		// 절두체 생성을 위한 원근 투영 행렬
		// 만약 모델링 행렬, 뷰 행렬, 원근 투영 행렬을 모두 사용하면 로컬 공산 좌표를 사용해 절두체 컬링이 가능
		Matrix4x4 ptMatrix = GetPerspectiveMatrix().Transpose();

		// 원근 투영 행렬의 벡터를 계산한 4차원 벡터(a, b, c, d)를 뷰 공간의 점(x, y, z, 1)과 내적하면
		// 평면의 방정식과 동일한 형태가 나옴 (ax + by + cz + d)
		std::array<Plane, 6> frustumPlanes = {
			// 이렇게 구한 평면의 방향은 모두 절두체 내부를 향하므로 부호를 반전시켜 외부를 향하도록 함
			Plane(-(ptMatrix[3] + ptMatrix[1])), // 좌측(-Y)
			Plane(-(ptMatrix[3] - ptMatrix[1])), // 우측(+Y)
			Plane(-(ptMatrix[3] + ptMatrix[0])), // 하단(-X)
			Plane(-(ptMatrix[3] - ptMatrix[0])), // 상단(+X)
			Plane(-(ptMatrix[3] + ptMatrix[2])),  // 근평면(-Z)
			Plane(-(ptMatrix[3] - ptMatrix[2])) // 원평면(+Z)
		};

		_viewFrustum = Frustum(frustumPlanes);
	}

	void CameraComponent::CreateFrustum(const Matrix4x4& mat)
	{
		// 로컬 공간 데이터를 받아 절두체 컬링이 가능한 절두체 생성
		Matrix4x4 mvptMatrix = (mat * GetViewPerspectiveMatrix()).Transpose();

		// 로컬 공간의 평면
		std::array<Plane, 6> frustumPlanes = {
			// 이렇게 구한 평면의 방향은 모두 절두체 내부를 향하므로 부호를 반전시켜 외부를 향하도록한다.
			Plane(-(mvptMatrix[3] + mvptMatrix[1])), // 좌측(-Y)
			Plane(-(mvptMatrix[3] - mvptMatrix[1])), // 우측(+Y)
			Plane(-(mvptMatrix[3] + mvptMatrix[0])), // 하단(-X)
			Plane(-(mvptMatrix[3] - mvptMatrix[0])), // 상단(+X)
			Plane(-(mvptMatrix[3] + mvptMatrix[2])),  // 근평면(-Z)
			Plane(-(mvptMatrix[3] - mvptMatrix[2])) // 원평면(+Z)
		};

		_viewFrustum = Frustum(frustumPlanes);
	}

	void CameraComponent::SetLookAt(const Vector3& targetPos, const Vector3& up)
	{
		_cameraTransform->SetRotation(Quaternion(targetPos - _cameraTransform->GetPos(), up));
	}

	void CameraComponent::SetLookAt(Object* object, const Vector3& up)
	{
		TransformComponent* tr = object->GetComponent<TransformComponent>();
		if (tr) {
			SetLookAt(tr->GetPos(), up);
		}
	}

	const Vector3 CameraComponent::GetViewDir() const
	{
		return _cameraTransform->GetLocalZ().GetNormalize();
	}

	Matrix4x4 CameraComponent::GetViewMatrix() const
	{
		// 이동 역변환 -> 회전 역변환
		//return _cameraTransform->GetInvTranslationMatrix() * _cameraTransform->GetInvRotationMatrix();

		Vector3 viewX, viewY, viewZ;
		GetViewAxes(viewX, viewY, viewZ);
		Vector3 pos = _cameraTransform->GetPos();

		return Matrix4x4(
			Vector4(Vector3(viewX.X, viewY.X, viewZ.X), 0.f),
			Vector4(Vector3(viewX.Y, viewY.Y, viewZ.Y), 0.f),
			Vector4(Vector3(viewX.Z, viewY.Z, viewZ.Z), 0.f),
			Vector4(-viewX.Dot(pos), -viewY.Dot(pos), -viewZ.Dot(pos), 1.f)
		);
	}

	Matrix4x4 CameraComponent::GetViewRotationMatrix() const
	{
		//return _cameraTransform->GetInvRotationMatrix();

		Vector3 viewX, viewY, viewZ;
		GetViewAxes(viewX, viewY, viewZ);
		Vector3 pos = _cameraTransform->GetPos();

		return Matrix4x4(
			Vector4(Vector3(viewX.X, viewY.X, viewZ.X), 0.f),
			Vector4(Vector3(viewX.Y, viewY.Y, viewZ.Y), 0.f),
			Vector4(Vector3(viewX.Z, viewY.Z, viewZ.Z), 0.f),
			Vector4::UnitW
		);
	}

	Matrix4x4 CameraComponent::GetPerspectiveMatrix() const
	{
		// 카메라 초점거리 (Focus Length)
		float d = 1.f / (std::tanf(MathUtils::Deg2Rad(_fov) * 0.5f));
		float invA = 1.f / _viewportSize.GetAspectRatio(); // NDC를 화면 비율에 맞게 미리 처리

		// 깊이 값의 범위는 [0, 1]
		float invNF = 1.f / (_farZ - _nearZ);
		float k = _farZ * invNF;
		float l = -(_nearZ * _farZ) * invNF;

		return Matrix4x4(
			Vector4(d * invA, 0.f, 0.f, 0.f),
			Vector4(0.f, d, 0.f, 0.f),
			Vector4(0.f, 0.f, k, 1.f),
			Vector4(0.f, 0.f, l, 0.f)
		);
	}

	Matrix4x4 CameraComponent::GetViewPerspectiveMatrix() const
	{
		return GetViewMatrix() * GetPerspectiveMatrix();
	}

	Matrix4x4 CameraComponent::GetOrthographicMatrix() const
	{
		const float halfWidth = _viewportSize.X * 0.5f;
		const float halfHeight = _viewportSize.Y * 0.5f;
		const float invNF = 1.0f / (_farZ - _nearZ);  // scale factor

		return Matrix4x4(
			Vector4(1.0f / halfWidth, 0.0f, 0.0f, 0.0f),
			Vector4(0.0f, 1.0f / halfHeight, 0.0f, 0.0f),
			Vector4(0.0f, 0.0f, invNF, 0.0f),         // scale: 1/(far-near)
			Vector4(0.0f, 0.0f, -_nearZ * invNF, 1.0f) // offset: -near/(far-near)
		);
	}

	void CameraComponent::GetViewAxes(Vector3& outViewX, Vector3& outViewY, Vector3& outViewZ) const
	{
		outViewX = _cameraTransform->GetLocalX();
		outViewY = _cameraTransform->GetLocalY();
		outViewZ = _cameraTransform->GetLocalZ();
	}
}