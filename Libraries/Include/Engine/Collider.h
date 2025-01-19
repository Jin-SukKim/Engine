#pragma once
#include "Component.h"
#include "CollisionDelegate.h"

namespace JE {
	class Collider : public Component
	{
		using Super = Component;
	public:
		Collider(ColliderType type, const std::wstring& name);
		virtual ~Collider() override {};

		virtual void Render(IRenderer* r) override;

		virtual void OnCollisionBeginOverlap(Collider* other);
		virtual void OnCollisionOverlap(Collider* other);
		virtual void OnCollisionEndOverlap(Collider* other);

		virtual bool CheckCollision(Collider* other);

		uint32 GetID() const { return _collisionID; }
		void SetOffset(const Vector3& offset) { _offset = offset; }
		Vector3 GetOffset() const { return _offset; }
		void SetColliderType(const ColliderType& type) { _type = type; }
		ColliderType GetColliderType() const { return _type; }

		void SetCollisionEnable() { _enable = true; }
		void SetCollisionDisable() { _enable = false; }
		bool GetCollisionEnable() const { return _enable; }

	public:
		// �Լ��� �����ؼ� Delegate�� Bind�� ���
		CollisionDelegate BeginOverlap;
		CollisionDelegate Overlap;
		CollisionDelegate EndOverlap;

	private:
		static uint32 _collisionID; // collider�� ������ �� ���� 1�� ����
		uint32 _id;
		Vector3 _offset = Vector3::Zero; // �浹ü�� ��ġ�� Mesh�� ��ġ���� ����
		Vector3 _size = Vector3::One;
		ColliderType _type;
		bool _enable = true;

		// Collider Delegate

	};
}
