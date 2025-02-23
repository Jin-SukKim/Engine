#include "pch.h"
#include "Mesh.h"

namespace JE {
	bool Mesh::Load(const std::wstring& path)
	{
		// TODO: Mesh Load
		return true;
	}
	bool Mesh::Load(const std::vector<Vertex3D>& vertices, const std::vector<uint32>& indices)
	{
		_vertices = vertices;
		_indices = indices;
		return false;
	}
}