#pragma once
#include "Collider.h"
#include "Mesh2DComponent.h"

namespace JE{
	class CircleComponent : public Collider
	{
		using Super = Collider;
	public:
		CircleComponent(const std::wstring& name) : Super(ColliderType::Circle, name) {}
		virtual ~CircleComponent() override {};

		void Init() override;
		void Render(IRenderer* r) override;
		bool CheckCollision(Collider* other) override;

		float GetRadius() const { return _radius; }
		void SetRadius(float radius) { _radius = radius; }

		Vector2 GetPos() const;
	private:
		float _radius = 1.0f;
		std::unique_ptr<Mesh2DComponent> _mesh = nullptr;
	};
}
