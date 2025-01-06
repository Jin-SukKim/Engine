#pragma once
#include "Component.h"

namespace JE {
	class Mesh2D;
	struct Color;
	class Texture;

	class Mesh2DComponent : public Component
	{
		using Super = Component;
	public:
		Mesh2DComponent(const std::wstring& name) : Super(name, ComponentType::Mesh) {}
		virtual ~Mesh2DComponent() override {}

		virtual void Tick(const float& DeltaTime) override;
		virtual void Render() override;

		void SetMesh(const std::wstring& name);
		Mesh2D* GetMesh() const { return _mesh; }

		void SetTexture(const std::wstring& name);
		Texture* GetTexture() const { return _texture; }
	
		// Ambient Color
		void SetAmbient(Color ambient) { _ambient = ambient; }
		Color& GetAmbient() { return _ambient; }
		const Color& GetAmbient() const { return _ambient; }
		
		// Diffuse Color
		void SetDiffuse(Color diffuse) { _diffuse = diffuse; }
		Color& GetDiffuse() { return _diffuse; }
		const Color& GetDiffuse() const { return _diffuse; }
		
		// Specular Color
		void SetSpecular(Color specular) { _specular = specular; }
		Color& GetSpecular() { return _specular; }
		const Color& GetSpecular() const { return _specular; }

	private:
		Mesh2D* _mesh = nullptr;
		Texture* _texture = nullptr;

		Color _ambient = Color::Black;
		Color _diffuse = Color::Black;
		Color _specular = Color::Black;
		float _alpha = 10.f;
	};
}
