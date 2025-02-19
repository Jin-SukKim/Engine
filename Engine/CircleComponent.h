#pragma once
#include "Collider.h"

namespace JE{
	class Mesh2D;
	struct Circle;
	class Transform2DComponent;

	class CircleComponent : public Collider
	{
		using Super = Collider;
	public:
		CircleComponent(const std::wstring& name) : Super(ColliderType::Circle, name) {}
		virtual ~CircleComponent() override {};

		void Init() override;
		virtual void Tick(const float& DeltaTime) override;
		void Render(IRenderer* r) override;
		bool CheckCollision(Collider* other) override;

		void SetRadius(float radius) { _bound.Radius = radius; }

		void UpdateCircle();
		const Circle& GetCircle() {	return _bound; }

	private:
		Circle _bound;
		Mesh2D* _mesh = nullptr;
		std::unique_ptr<Transform2DComponent> _transform;
	};
}
