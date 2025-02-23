#pragma once

namespace JE {
	/*
		절두체에 맞게 삼각형을 잘라내는 삼각형 Clipping으로 한 평면씩 테스트와 정점의 투영을 진행
		총 7개의 TriangleClipping 구조체를 가지고 각각의 절두체 평면에 테스트를 진행
		TriangleClipping 구조체 하나당 평면 하나의 테스트
	*/
	struct TriangleClipping
	{
		TriangleClipping(std::function<bool(const Vertex3D& vertex)> testFunc,
			std::function<Vertex3D(const Vertex3D& startVertex, const Vertex3D& endVertex)> edgeFunc)
			: ClippingTestFunc(testFunc), GetEdgeVertexFunc(edgeFunc), TestResult({false, false, false}) {
		}

		// 삼각형을 Clipping하는 함수 : Parameter로 받는 정점의 배열에 삼각형이 분할하면 저장
		void ClipTriangle(std::vector<Vertex3D>& InOutVertices);

	private:
		// 점 하나가 평면의 바깥에 있어 삼각형이 2개로 쪼개지는 경우 (한 점이 두 점으로 투영되 두 개의 삼각형으로 쪼개지며 사각형이 생성)
		void divideIntoTwoTriangles(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount);
		// 점 두 개가 평면의 바깥에 있어 삼각형의 두 점이 변하는 경우 (정점의 위치만 변하지 삼각형은 동일하게 하나)
		void clipTriangle(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount);
	public:
		// 해당 점이 평면 바깥에 있는지 판정하는 함수 객체
		std::function<bool(const Vertex3D& vertex)> ClippingTestFunc;
		// 두 점으로 만들어진 선분이 평면에 교차하는 점을 구하는 함수 객체 (점을 평면에 투영)
		std::function<Vertex3D(const Vertex3D& startVertex, const Vertex3D& endVertex)> GetEdgeVertexFunc;
		// 판정 결과를 보관하는 변수 (true인 경우 점이 외부에 있는 것)
		std::array<bool, 3> TestResult;
	};

}