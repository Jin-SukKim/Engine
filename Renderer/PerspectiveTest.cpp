#include "pch.h"
#include "PerspectiveTest.h"

namespace JE {
	void PerspectiveTest::ClipTriangle(std::vector<Vertex3D>& InOutVertices)
	{
		// 전체 삼각형의 수
		size_t triangles = InOutVertices.size() / 3;
		// 삼각형의 수만큼 루프 돌기
		for (size_t ti = 0; ti < triangles; ++ti) {
			size_t startIndex = ti * 3; // 삼각형의 Index
			size_t nonPassCount = 0; // 평면 바깥에 위치한 점의 수를 보관하는 변수

			// 삼각형의 세 점을 돌면서 절두체 평면의 테스트 시작
			for (size_t i = 0; i < 3; ++i) {
				// 테스트 결과를 저장
				TestResult[i] = ClippingTestFunc(InOutVertices[startIndex + i]);
				if (TestResult[i]) // 점이 평면의 바깥에 위치한 경우
					++nonPassCount;
			}

			// 삼각형의 세 정점이 모두 절두체 안에 삼각형 클리핑 건너뛰기
			if (nonPassCount == 0)
				continue;
			// 한 정점이 평면의 바깥에 있는 경우 삼각형을 두 개로 쪼개기
			else if (nonPassCount == 1)
				divideIntoTwoTriangles(InOutVertices, startIndex, nonPassCount);
			// 두 정점이 평면의 바깥에 있는 경우 삼각형의 정점 값을 변경
			else if (nonPassCount == 2)
				clipTriangle(InOutVertices, startIndex, nonPassCount);
			// 세 정점이 모두 바깥에 위치하면 삼각형의 정점을 제거
			else {
				// 삼각형 제거하고 제거되지 않은 삼각형만 추후 렌더링
				InOutVertices.erase(InOutVertices.begin() + startIndex, InOutVertices.begin() + startIndex + 3);
				--triangles;
				--ti;
			}
		}
	}

	void PerspectiveTest::divideIntoTwoTriangles(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount)
	{
		// 평면의 바깥에 위치한 점의 index 찾기
		uint8 index = 0;
		// 삼각형의 첫번째 정점이 아니라면
		if (!TestResult[0])
			index = TestResult[1] ? 1 : 2; // 2번째 or 3번째

		// 평면의 안쪽에 위치한 다른 두 점의 index
		size_t v1Index = startIndex + (index + 1) % 3;
		size_t v2Index = startIndex + (index + 2) % 3;

		// 안쪽에 위치한 다른 두 정점
		Vertex3D v1 = vertices[v1Index];
		Vertex3D v2 = vertices[v2Index];

		// 바깥에 위치한 점을 평면에 투영시킨 새로운 두 정점, 즉 두 선분의 평면과의 교차지점
		Vertex3D clipped1 = GetEdgeVertexFunc(vertices[startIndex + index], v1);
		Vertex3D clipped2 = GetEdgeVertexFunc(vertices[startIndex + index], v2);

		// 교차지점(새로운 두 정점)을 사용해 삼각형을 두 개로 분할 -> 삼각형 2개로 이뤄진 사각형의 형태가 됨 
		// 삼각형 1
		vertices[startIndex] = clipped1;
		vertices[startIndex + 1] = v1;
		vertices[startIndex + 2] = v2;
		// 삼각형 2
		vertices.emplace_back(clipped1);
		vertices.emplace_back(v2);
		vertices.emplace_back(clipped2);
	}

	void PerspectiveTest::clipTriangle(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount)
	{
		// 평면의 안쪽에 위치한 점의 index 찾기 (두 개의 정점이 바깥에 위치)
		uint8 index = 0;
		if (TestResult[0])
			index = !TestResult[1] ? 1 : 2;

		// 평면의 바깥에 위치한 두 정점의 index
		size_t v1Index = startIndex + (index + 1) % 3;
		size_t v2Index = startIndex + (index + 2) % 3;

		// 평면의 바깥에 위치한 두 정점
		Vertex3D v1 = vertices[v1Index];
		Vertex3D v2 = vertices[v2Index];

		// 바깥에 위치한 두 점을 평면에 투영, 즉 두 선분의 평면과의 교차지점
		Vertex3D clipped1 = GetEdgeVertexFunc(vertices[startIndex + index], v1);
		Vertex3D clipped2 = GetEdgeVertexFunc(vertices[startIndex + index], v2);

		// 바깥에 위치한 두 점을 절두체 평면 안으로 이동
		vertices[v1Index] = clipped1;
		vertices[v2Index] = clipped2;
	}
}