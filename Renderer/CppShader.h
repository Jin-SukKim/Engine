#pragma once

namespace JE {
	__forceinline void VertexShader2D(std::vector<Vertex2D>& vertices, const Matrix3x3& mat) {
		for (Vertex2D& v : vertices) {
			v.Pos = v.Pos * mat;
		}
	}

	__forceinline void VertexShader3D(std::vector<Vertex3D>& vertices, const Matrix4x4& mat) {
		for (Vertex3D& v : vertices) {
			// Local -> World -> View -> Clip으로 공간 변환
			v.Pos = v.Pos * mat;
		}
	}

	// Color : Actor 자체의 색, ColorParam : 기본 white or light의 색
	__forceinline Color PixelShader(Color& color, const Color& colorParam) {
		return color * colorParam;
	}
}