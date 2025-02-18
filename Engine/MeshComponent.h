#pragma once
#include "Component.h"

namespace JE {
	class Mesh;
	class Texture;

	class MeshComponent : public Component
	{
		using Super = Component;
	public:
		MeshComponent(const std::wstring& name) : Super(name, ComponentType::Mesh) {}
		virtual ~MeshComponent() override {}

		virtual void Tick(const float& DeltaTime) override;
		virtual void Render(IRenderer* r) override;

		void SetMesh(const std::wstring& name);
		Mesh* GetMesh() const { return _mesh; }

		void SetTexture(const std::wstring& name);
		Texture* GetTexture() const { return _texture; }

	private:
		Mesh* _mesh = nullptr;
		Texture* _texture = nullptr;
	};
}
