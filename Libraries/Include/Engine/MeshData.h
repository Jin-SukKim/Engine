#pragma once

namespace JE {
	namespace MeshData {
		// Mesh를 구성하는 정점 배열과 인덱스 배열 생성
		static const std::vector<Vertex2D> SquareVertex = {
			Vertex2D(Vector2(-1.f, -1.f), Color::Black, Vector2(0.f, 1.f)), // 좌하단 정점
			Vertex2D(Vector2(-1.f, 1.f), Color::Black, Vector2(0.f, 0.f)), // 좌상단 정점
			Vertex2D(Vector2(1.f, 1.f), Color::Black, Vector2(1.f, 0.f)), // 우상단 정점
			Vertex2D(Vector2(1.f, -1.f), Color::Black, Vector2(1.f, 1.f)) // 우하단 정점
		};

		static const std::vector<uint32> SquareIndices = {
			0, 1, 2,
			0, 2, 3
		};

		static const std::vector<Vertex3D> PlaneVertex = {
			Vertex3D(Vector3(-1.f, 0.f, -1.f)), // 좌하단 정점
			Vertex3D(Vector3(-1.f, 0.f, 1.f)), // 좌상단 정점
			Vertex3D(Vector3(1.f, 0.f, 1.f)), // 우상단 정점
			Vertex3D(Vector3(1.f, 0.f, -1.f)) // 우하단 정점
		};

		static const std::vector<uint32> PlaneIndices = {
			0,  1,  2,
			0,  2,  3
		};

		static const std::vector<Vertex3D> BoxVertex = {
			// Top
			Vertex3D(Vector3(-1.f, 1.f, -1.f)), Vertex3D(Vector3(-1.f, 1.f, 1.f)), Vertex3D(Vector3(1.f, 1.f, 1.f)), Vertex3D(Vector3(1.f, 1.f, -1.f)),
			// Bottom
			Vertex3D(Vector3(-1.f, -1.f, -1.f)), Vertex3D(Vector3(1.f, -1.f, -1.f)), Vertex3D(Vector3(1.f, -1.f, 1.f)), Vertex3D(Vector3(-1.f, -1.f, 1.f)),
			// Front
			Vertex3D(Vector3(-1.f, -1.f, -1.f)), Vertex3D(Vector3(-1.f, 1.f, -1.f)), Vertex3D(Vector3(1.f, 1.f, -1.f)), Vertex3D(Vector3(1.f, -1.f, -1.f)),
			// Back
			Vertex3D(Vector3(-1.f, -1.f, 1.f)), Vertex3D(Vector3(1.f, -1.f, 1.f)), Vertex3D(Vector3(1.f, 1.f, 1.f)), Vertex3D(Vector3(-1.f, 1.f, 1.f)),
			// Left
			Vertex3D(Vector3(-1.f, -1.f, 1.f)), Vertex3D(Vector3(-1.f, 1.f, 1.f)), Vertex3D(Vector3(-1.f, 1.f, -1.f)), Vertex3D(Vector3(-1.f, -1.f, -1.f)),
			// Right
			Vertex3D(Vector3(1.f, -1.f, 1.f)), Vertex3D(Vector3(1.f, -1.f, -1.f)), Vertex3D(Vector3(1.f, 1.f, -1.f)), Vertex3D(Vector3(1.f, 1.f, 1.f))
		};

		static const std::vector<uint32> BoxIndice = {
			0,  1,  2,  0,  2,  3,  // Top
			4,  5,  6,  4,  6,  7,  // Bottom
			8,  9,  10, 8,  10, 11, // Front
			12, 13, 14, 12, 14, 15, // Back
			16, 17, 18, 16, 18, 19, // Left
			20, 21, 22, 20, 22, 23  // Right
		};
	}
}