#pragma once
#include "Collider.h"

namespace JE {
    struct Rectangle;
    class Mesh2D;
    class Transform2DComponent;

    class RectangleComponent : public Collider
    {
        using Super = Collider;
    public:
        RectangleComponent(const std::wstring& name) : Super(ColliderType::Rectangle, name) {}
		virtual ~RectangleComponent() override {};

        void Init() override;
        virtual void Tick(const float& DeltaTime) override;
        void Render(IRenderer* r) override;
		bool CheckCollision(Collider* other) override;

        void UpdateRect();
        const Rectangle& GetRect() { return _bound; };

    private:
		Vector2 _size = Vector2::One;
        Rectangle _bound;
        Mesh2D* _mesh = nullptr;
        std::unique_ptr<Transform2DComponent> _transform = nullptr;
    };
}
