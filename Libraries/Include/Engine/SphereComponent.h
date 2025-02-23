#pragma once
#include "Collider.h"

namespace JE {
    struct Sphere;
    class Mesh;
    class TransformComponent;

    class SphereComponent : public Collider
    {
        using Super = Collider;
    public:
        SphereComponent(const std::wstring& name) : Super(ColliderType::Sphere, name) {}
        virtual ~SphereComponent() override {};

        void Init() override;
        virtual void Tick(const float& DeltaTime) override;
        void Render(IRenderer* r) override;
        bool CheckCollision(Collider* other) override;

        void UpdateSphere();
        const Sphere& GetSphere() { return _bound; };

    private:
        Vector3 _size = Vector3::One;
        Sphere _bound;
        Mesh* _mesh = nullptr;
        std::unique_ptr<TransformComponent> _transform = nullptr;
    };
}
