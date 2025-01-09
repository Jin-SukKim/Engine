#pragma once

namespace JE {
	/*
		절두체에 맞게 삼각형을 잘라내는 삼각형 Clipping으로 한 평면씩 테스트와 정점의 투영을 진행
		총 7개의 TriangleClipping 구조체를 가지고 각각의 절두체 평면에 테스트를 진행
		TriangleClipping 구조체 하나당 평면 하나의 테스트
	*/
	struct PerspectiveTest
	{
		PerspectiveTest(std::function<bool(const Vertex3D& vertex)> testFunc,
			std::function<Vertex3D(const Vertex3D& startVertex, const Vertex3D& endVertex)> edgeFunc)
			: ClippingTestFunc(testFunc), GetEdgeVertexFunc(edgeFunc), TestResult({false, false, false}) {}

		// 삼각형을 Clipping하는 함수 : Parameter로 받는 정점의 배열에 삼각형이 분할하면 저장
		void ClipTriangle(std::vector<Vertex3D>& InOutVertices);

	private:
		// 점 하나가 평면의 바깥에 있어 삼각형이 2개로 쪼개지는 경우 (한 점이 두 점으로 투영되 두 개의 삼각형으로 쪼개지며 사각형이 생성)
		void divideIntoTwoTriangles(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount);
		// 점 두 개가 평면의 바깥에 있어 삼각형의 두 점이 변하는 경우 (정점의 위치만 변해 삼각형은 동일하게 하나)
		void clipTriangle(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount);
	public:
		// 해당 점이 평면 바깥에 있는지 판정하는 함수 객체
		std::function<bool(const Vertex3D& vertex)> ClippingTestFunc;
		// 두 점으로 만들어진 선분이 평면에 교차하는 점을 구하는 함수 객체 (점을 평면에 투영)
		std::function<Vertex3D(const Vertex3D& startVertex, const Vertex3D& endVertex)> GetEdgeVertexFunc;
		// 판정 결과를 보관하는 변수 (true인 경우 점이 외부에 있는 것)
		std::array<bool, 3> TestResult;
	};

	// 클립 좌표에서 뷰공간의 깊이값인 W값이 0보다 작으면 카메라 뒤에 있다는 의미로 정점이 카메라보다 뒤에 있는지 판별
	// (클립 좌표의 원점은 카메라의 위치)
	static std::function<bool(Vertex3D)> TestFuncW0 = [](const Vertex3D& vertex) {
		// 카메라보다 뒤에 있는지 판별
		return vertex.Pos.W < 0.f;
	};

	// w값이 0일때의 새로운 점 구하기, 즉 카메라 뒤에 있는 정점을 카메라가 있는 w = 0의 평면에 투영
	static std::function<Vertex3D(Vertex3D, Vertex3D)> EdgeFuncW0 = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W;
		float p2 = endVertex.Pos.W;
		float t = p1 / (p1 - p2);
		// 아핀 결합의 직선의 방정식 P3 = (1.f - t) * P1 + t * P2;
		return startVertex * (1.f - t) + endVertex * t;
	};

	// 정점이 절두체 상단의 평면의 외부에 있는지 판별
	static auto TestFuncPY = [](const Vertex3D& vertex) {
		return vertex.Pos.Y > vertex.Pos.W;
		};

	// 정점을 절두체 상단의 평면에 투영
	static auto EdgeFuncPY = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W - startVertex.Pos.Y;
		float p2 = endVertex.Pos.W - endVertex.Pos.Y;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// 정점이 절두체의 하단 평면보다 아래에 있는지 판별 (즉, 외부에 있는지 판별)
	static auto TestFuncNY = [](const Vertex3D& vertex) {
		return vertex.Pos.Y < -vertex.Pos.W;
	};

	// 절두체 하단의 평면에 정점을 투영
	static auto EdgeFuncNY = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W + startVertex.Pos.Y;
		float p2 = endVertex.Pos.W + endVertex.Pos.Y;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// 정점이 절두체 우측의 평면의 외부에 있는지 판별
	static auto TestFuncPX = [](const Vertex3D& vertex) {
		return vertex.Pos.X > vertex.Pos.W;
	};

	// 정점을 절두체 우측 평면에 투영
	static auto EdgeFuncPX = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W - startVertex.Pos.X;
		float p2 = endVertex.Pos.W - endVertex.Pos.X;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// 정점이 절두체 좌측의 평면의 외부에 있는지 판별
	static auto TestFuncNX = [](const Vertex3D& vertex) {
		return vertex.Pos.X < -vertex.Pos.W;
	};

	// 정점을 절두체 우측 평면에 투영
	static auto EdgeFuncNX = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W + startVertex.Pos.X;
		float p2 = endVertex.Pos.W + endVertex.Pos.X;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// 정점이 절두체의 근평면의 외부에 있는지 판별
	static auto TestFuncNear = [](const Vertex3D& vertex) {
		return vertex.Pos.Z < -vertex.Pos.W;
	};

	// 정점을 절두체 근평면에 투영
	static auto EdgeFuncNear = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W + startVertex.Pos.Z;
		float p2 = endVertex.Pos.W + endVertex.Pos.Z;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// 정점이 절두체의 원평면의 외부에 있는지 판별
	static auto TestFuncFar = [](const Vertex3D& vertex) {
		return vertex.Pos.Z > vertex.Pos.W;
	};

	// 정점을 절두체의 원평면에 투영
	static auto EdgeFuncFar = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W - startVertex.Pos.Z;
		float p2 = endVertex.Pos.W - endVertex.Pos.Z;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};
}