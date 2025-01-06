#pragma once
#include "Resource.h"

namespace JE {
	class Mesh2D : public  Resource
	{
		using Super = Resource;
	public:
		Mesh2D(const std::wstring& name) : Super(name, ResourceType::Mesh) {}
		virtual ~Mesh2D() override {}

		bool Load(const std::wstring& path) override;

		void SetVertices(const std::vector<Vertex2D>& vertices) { _vertices = vertices; }
		std::vector<Vertex2D>& GetVertices() { return _vertices; }
		const std::vector<Vertex2D>& GetVertices() const { return _vertices; }

		void SetIndices(const std::vector<uint32>& indices) { _indices = indices; }
		std::vector<uint32>& GetIndices() { return _indices; }
		const std::vector<uint32>& GetIndices() const { return _indices; }

	private:
		std::vector<Vertex2D> _vertices; // 정점 버퍼
		std::vector<uint32> _indices; // 인덱스 버퍼 (크기 = 삼각형의 수 * 3)
	};
}

