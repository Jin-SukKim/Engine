#include "pch.h"
#include "Mesh2D.h"

namespace JE {
    bool Mesh2D::Load(const std::wstring& path)
    {
        // TODO: Mesh Load
        return true;
    }
    bool Mesh2D::Load(const std::vector<Vertex2D>& vertices, const std::vector<uint32>& indices)
    {
        _vertices = vertices;
        _indices = indices;
        return true;
    }
}

