#pragma once
#include "Collider.h"
#include "Mesh2DComponent.h"

namespace JE {
    class RectangleComponent : public Collider
    {
        using Super = Collider;
    public:
        RectangleComponent(const std::wstring& name) : Super(ColliderType::Rectangle, name) {}
		virtual ~RectangleComponent() override {};

        void Init() override;
        void Render(IRenderer* r) override;
		bool CheckCollision(Collider* other) override;

        RECT GetRect() const;

    private:
		Vector2 _size = Vector2::One;
        std::unique_ptr<Mesh2DComponent> _mesh = nullptr;
    };
}
