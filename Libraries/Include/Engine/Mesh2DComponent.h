#pragma once
#include "Component.h"

namespace JE {
	class Mesh2D;
	class Texture;

	class Mesh2DComponent : public Component
	{
		using Super = Component;
	public:
		Mesh2DComponent(const std::wstring& name) : Super(name, ComponentType::Mesh) {}
		virtual ~Mesh2DComponent() override {}

		virtual void Tick(const float& DeltaTime) override;
		virtual void Render(IRenderer* r) override;

		void SetMesh(const std::wstring& name);
		Mesh2D* GetMesh() const { return _mesh; }

		void SetTexture(const std::wstring& name);
		Texture* GetTexture() const { return _texture; }

	private:
		Mesh2D* _mesh = nullptr;
		Texture* _texture = nullptr;
	};
}
