#pragma once
#include "Component.h"
#include "CollisionDelegate.h"

namespace JE {
	class RectangleComponent;
	class CircleComponent;

	class Collider : public Component
	{
		using Super = Component;
	public:
		Collider(ColliderType type, const std::wstring& name);
		virtual ~Collider() override {};

		virtual void Init() override {}
		virtual void Render(IRenderer* r) override;

		// Set Collision Event
		virtual void OnCollisionBeginOverlap(Collider* other);
		virtual void OnCollisionOverlap(Collider* other);
		virtual void OnCollisionEndOverlap(Collider* other);

		virtual bool CheckCollision(Collider* other);

		uint32 GetID() const { return _collisionID; }
		void SetOffset(const Vector3& offset) { _offset = offset; }
		Vector3 GetOffset() const { return _offset; }
		void SetSize(const Vector3& size) { _size = size; }
		Vector3 GetSize() const { return _size; }
		void SetColliderType(const ColliderType& type) { _type = type; }
		ColliderType GetColliderType() const { return _type; }
		void SetVisible(bool show) { _show = show; }

		void SetCollisionEnable() { _enable = true; }
		void SetCollisionDisable() { _enable = false; }
		bool GetCollisionEnable() const { return _enable; }

	protected:
		bool CheckCollisionRectToRect(RectangleComponent* b1, RectangleComponent* b2);
		bool CheckCollisionCircleToRect(CircleComponent* c1, RectangleComponent* b1);
		bool CheckCollisionCircleToCircle(CircleComponent* c1, CircleComponent* c2);

		// ���� ���ȿ� �ִ��� Ȯ��
		bool CheckPointInCircle(const Vector2& cPos, const float& radius, const Vector2& point);
	public:
		// Collider Delegate
		// �Լ��� �����ؼ� Delegate�� Bind�� ���
		CollisionDelegate BeginOverlap;
		CollisionDelegate Overlap;
		CollisionDelegate EndOverlap;

	private:
		static uint32 _collisionID; // collider�� ������ �� ���� 1�� ����
		// Collider ID
		uint32 _id;
		// Collider Position from Actor
		Vector3 _offset = Vector3::Zero; // �浹ü�� ��ġ�� Mesh�� ��ġ���� ����
		// Collider Size
		Vector3 _size = Vector3::One;
		ColliderType _type;
		// Collision Enable
		bool _enable = true;
		// Render Enable
		bool _show = false;
	};
}
