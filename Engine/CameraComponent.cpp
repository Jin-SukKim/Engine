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

				// �÷��̾ �� ���� �ʰ� ���󰡴� �ð��� ȿ���� Lagging ȿ��
				// minDistance == 0�̸� Lagging ȿ������ �ٷ� ���󰡱�
				if (_minDistance == 0.f || (ownerPos - cameraPos).SizeSquared() < _minDistance * _minDistance)
					_cameraTransform->SetPos(ownerPos);
				else {
					// ���� �����ӿ��� �Ѿư� ���� ���
					float ratio = std::clamp(_lerpSpeed * DeltaTime, 0.f, 1.f);
					// ī�޶��� ���� ��ġ���� �÷��̾��� ���� ��ġ���� �̵��ϴ� ���͸� ���ϰ� ratio�� ���� �̵��� ��ġ ���
					Vector3 newPos = MathUtils::Lerp(cameraPos, ownerPos, ratio);
					_cameraTransform->SetPos(newPos);
				}
			}
		}
	}

	void CameraComponent::CreateFrustum()
	{
		// ����� �������� ����� ����ü ����
		/*
		float halfFOV = GetFOV() * 0.5f;
		float sin = 0.f, cos = 0.f;
		MathUtils::GetSinCos(sin, cos, halfFOV);

		// ���� ���ΰ� �����ϴٰ� �����ϰ� ������⿡ ����, ���� ������ ���� ����
		std::array<Plane, 6> frustumPlanes = {
			// �����¿� ����� ������ �о����� �� �������� �Ÿ��� 0�̹Ƿ� d�� 0���� ����
			// �����¿� ����� ī�޶� ���̵��� ȸ���Ǿ� ����
			Plane(Vector3(cos, 0.f, -sin), 0.f), // ���� (-Y)
			Plane(Vector3(-cos, 0.f, -sin), 0.f), // ���� (+Y)
			Plane(Vector3(0.f, -cos, -sin), 0.f), // �ϴ� (-X)
			Plane(Vector3(0.f, cos, -sin), 0.f), // ��� (+X)
			// �����, ������� �������� �Ÿ��� nearZ, farZ�̰� ������� ���� ���ʹ� -normal, ������� + normal
			// ������� ����� �ո��� ������ ���ϹǷ� d�� ���,
			// ������� ������ �־����� �����̱⿡ d�� ����
			Plane(-Vector3::UnitZ, _nearZ), // �����(-Z)
			Plane(Vector3::UnitZ, -_farZ) // �����(+Z)
		};
		*/
		
		// ���� ���� ����� Ȱ���� �� �����ϰ� ���� ����ü �ø��� ���� ����ü ����
		// ����ü ������ ���� ���� ���� ���
		// ���� �𵨸� ���, �� ���, ���� ���� ����� ��� ����ϸ� ���� ���� ��ǥ�� ����� ����ü �ø��� ����
		Matrix4x4 ptMatrix = GetPerspectiveMatrix().Transpose();

		// ���� ���� ����� ���͸� ����� 4���� ����(a, b, c, d)�� �� ������ ��(x, y, z, 1)�� �����ϸ�
		// ����� �����İ� ������ ���°� ���� (ax + by + cz + d)
		std::array<Plane, 6> frustumPlanes = {
			// �̷��� ���� ����� ������ ��� ����ü ���θ� ���ϹǷ� ��ȣ�� �������� �ܺθ� ���ϵ��� ��
			Plane(-(ptMatrix[3] + ptMatrix[1])), // ����(-Y)
			Plane(-(ptMatrix[3] - ptMatrix[1])), // ����(+Y)
			Plane(-(ptMatrix[3] + ptMatrix[0])), // �ϴ�(-X)
			Plane(-(ptMatrix[3] - ptMatrix[0])), // ���(+X)
			Plane(-(ptMatrix[3] + ptMatrix[2])),  // �����(-Z)
			Plane(-(ptMatrix[3] - ptMatrix[2])) // �����(+Z)
		};

		_viewFrustum = Frustum(frustumPlanes);
	}

	void CameraComponent::CreateFrustum(const Matrix4x4& mat)
	{
		// ���� ���� �����͸� �޾� ����ü �ø��� ������ ����ü ����
		Matrix4x4 mvptMatrix = (mat * GetViewPerspectiveMatrix()).Transpose();

		// ���� ������ ���
		std::array<Plane, 6> frustumPlanes = {
			// �̷��� ���� ����� ������ ��� ����ü ���θ� ���ϹǷ� ��ȣ�� �������� �ܺθ� ���ϵ����Ѵ�.
			Plane(-(mvptMatrix[3] + mvptMatrix[1])), // ����(-Y)
			Plane(-(mvptMatrix[3] - mvptMatrix[1])), // ����(+Y)
			Plane(-(mvptMatrix[3] + mvptMatrix[0])), // �ϴ�(-X)
			Plane(-(mvptMatrix[3] - mvptMatrix[0])), // ���(+X)
			Plane(-(mvptMatrix[3] + mvptMatrix[2])),  // �����(-Z)
			Plane(-(mvptMatrix[3] - mvptMatrix[2])) // �����(+Z)
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
		// �̵� ����ȯ -> ȸ�� ����ȯ
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
		// ī�޶� �����Ÿ� (Focus Length)
		float d = 1.f / (std::tanf(MathUtils::Deg2Rad(_fov) * 0.5f));
		float invA = 1.f / _viewportSize.GetAspectRatio(); // NDC�� ȭ�� ������ �°� �̸� ó��

		// ���� ���� ������ [0, 1]
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