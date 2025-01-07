#pragma once

namespace JE {

// 2차원 정점
struct Vertex2D {
	Vertex2D() {}
	Vertex2D(const Vector2& pos) : Pos(pos) {}
	Vertex2D(const Vector2& pos, const Color & color) : Pos(pos), Color(color) {}
	Vertex2D(const Vector2& pos, const Vector2& uv) : Pos(pos), UV(uv) {}
	Vertex2D(const Vector2& pos, const Color& color, const Vector2& uv) : Pos(pos), Color(color), UV(uv) {};

	Vector2 Pos; // 좌표
	Color Color; // 색
	Vector2 UV;  // Texture 좌표계
};

// 3차원 정점
struct Vertex3D {
	Vertex3D() {}
	Vertex3D(const Vector4& pos) : Pos(pos) {}
	Vertex3D(const Vector4& pos, const Color & color) : Pos(pos), Color(color) {}
	Vertex3D(const Vector4& pos, const Vector2& uv) : Pos(pos), UV(uv) {}
	Vertex3D(const Vector4& pos, const Color& color, const Vector2& uv) : Pos(pos), Color(color), UV(uv) {};

	// 삼각형 클리핑에서 사용
	Vertex3D operator+(const Vertex3D& vertex) const {
		return Vertex3D(
			Pos + vertex.Pos,
			Color + vertex.Color,
			UV + vertex.UV
		);
	}

	// 삼각형 클리핑에서 사용
	Vertex3D operator*(float scalar) const {
		return Vertex3D(
			Pos * scalar,
			Color * scalar,
			UV * scalar
		);
	}

	Vector4 Pos; // 좌표
	Color Color; // 색
	Vector2 UV;  // Texture 좌표계
};
}