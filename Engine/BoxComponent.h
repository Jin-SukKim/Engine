#pragma once
#include "Collider.h"

namespace JE {
    struct Box;
    class Mesh;
    class TransformComponent;

    class BoxComponent : public Collider
    {
        using Super = Collider;
    public:
        BoxComponent(const std::wstring& name) : Super(ColliderType::Box, name) {}
        virtual ~BoxComponent() override {};

        void Init() override;
        virtual void Tick(const float& DeltaTime) override;
        void Render(IRenderer* r) override;
        bool CheckCollision(Collider* other) override;

        void UpdateBox();
        const Box& GetBox() { return _bound; };

    private:
        Vector3 _size = Vector3::One;
        Box _bound;
        Mesh* _mesh = nullptr;
        std::unique_ptr<TransformComponent> _transform = nullptr;
    };
}