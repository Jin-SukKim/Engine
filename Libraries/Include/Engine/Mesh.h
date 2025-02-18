#pragma once
#include "Resource.h"

namespace JE {
	struct Color;

	class Mesh : public Resource
	{
		using Super = Resource;
	public:
		Mesh(const std::wstring& name) : Super(name, ResourceType::Mesh) {}
		virtual ~Mesh() override {}

		bool Load(const std::wstring& path) override;
		bool Load(const std::vector<Vertex3D>& vertices, const std::vector<uint32>& indices);

		void SetVertices(const std::vector<Vertex3D>& vertices) { _vertices = vertices; }
		std::vector<Vertex3D>& GetVertices() { return _vertices; }
		const std::vector<Vertex3D>& GetVertices() const { return _vertices; }

		void SetIndices(const std::vector<uint32>& indices) { _indices = indices; }
		std::vector<uint32>& GetIndices() { return _indices; }
		const std::vector<uint32>& GetIndices() const { return _indices; }

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
		std::vector<Vertex3D> _vertices; // 정점 버퍼
		std::vector<uint32> _indices; // 인덱스 버퍼 (크기 = 삼각형의 수 * 3)

		Color _ambient = Color::Black;
		Color _diffuse = Color::Black;
		Color _specular = Color::Black;
		float _alpha = 10.f;
	};
}
