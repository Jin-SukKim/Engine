#pragma once
#include "Component.h"
#include "CollisionDelegate.h"

namespace JE {
	struct Rectangle;
	struct Circle;
	struct Box;
	struct Sphere;

	class Collider : public Component
	{
		using Super = Component;
	public:
		Collider(ColliderType type, const std::wstring& name);
		virtual ~Collider() override {};

		virtual void Init() override {}
		virtual void Tick(const float& DeltaTime) override {};
		virtual void Render(IRenderer* r) override;

		// Set Collision Event
		virtual void OnCollisionBeginOverlap(Collider* other);
		virtual void OnCollisionOverlap(Collider* other);
		virtual void OnCollisionEndOverlap(Collider* other);

		virtual bool CheckCollision(Collider* other);

		uint32 GetID() const { return _collisionID; }
		void SetOffset(const Vector3& offset) { _offset = offset; }
		Vector3 GetOffset() const { return _offset; }
		void SetScale(const Vector3& scale) { _scale = scale; }
		Vector3 GetScale() const { return _scale; }
		void SetColliderType(const ColliderType& type) { _type = type; }
		ColliderType GetColliderType() const { return _type; }
		void SetVisible(bool show) { _show = show; }

		void SetCollisionEnable() { _enable = true; }
		void SetCollisionDisable() { _enable = false; }
		bool GetCollisionEnable() const { return _enable; }

	protected:
		bool CheckCollisionCircleToRect(const Circle& c1, const Rectangle& b1);
		bool CheckCollisionSphereToBox(const Sphere& s1, const Box& b1);
	public:
		// Collider Delegate
		// 함수를 정의해서 Delegate에 Bind해 사용
		CollisionDelegate BeginOverlap;
		CollisionDelegate Overlap;
		CollisionDelegate EndOverlap;

	private:
		static uint32 _collisionID; // collider가 생성될 때 마다 1씩 증가
		// Collider ID
		uint32 _id;
		// Collider Position from Actor
		Vector3 _offset = Vector3::Zero; // 충돌체의 위치는 Mesh의 위치에서 시작
		// Collider scale
		Vector3 _scale = Vector3::One;
		ColliderType _type;
		// Collision Enable
		bool _enable = true;
		// Render Enable
		bool _show = false;
	};
}
