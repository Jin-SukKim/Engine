#pragma once

namespace JE {
	/*
		����ü�� �°� �ﰢ���� �߶󳻴� �ﰢ�� Clipping���� �� ��龿 �׽�Ʈ�� ������ ������ ����
		�� 7���� TriangleClipping ����ü�� ������ ������ ����ü ��鿡 �׽�Ʈ�� ����
		TriangleClipping ����ü �ϳ��� ��� �ϳ��� �׽�Ʈ
	*/
	struct PerspectiveTest
	{
		PerspectiveTest(std::function<bool(const Vertex3D& vertex)> testFunc,
			std::function<Vertex3D(const Vertex3D& startVertex, const Vertex3D& endVertex)> edgeFunc)
			: ClippingTestFunc(testFunc), GetEdgeVertexFunc(edgeFunc), TestResult({false, false, false}) {}

		// �ﰢ���� Clipping�ϴ� �Լ� : Parameter�� �޴� ������ �迭�� �ﰢ���� �����ϸ� ����
		void ClipTriangle(std::vector<Vertex3D>& InOutVertices);

	private:
		// �� �ϳ��� ����� �ٱ��� �־� �ﰢ���� 2���� �ɰ����� ��� (�� ���� �� ������ ������ �� ���� �ﰢ������ �ɰ����� �簢���� ����)
		void divideIntoTwoTriangles(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount);
		// �� �� ���� ����� �ٱ��� �־� �ﰢ���� �� ���� ���ϴ� ��� (������ ��ġ�� ���� �ﰢ���� �����ϰ� �ϳ�)
		void clipTriangle(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount);
	public:
		// �ش� ���� ��� �ٱ��� �ִ��� �����ϴ� �Լ� ��ü
		std::function<bool(const Vertex3D& vertex)> ClippingTestFunc;
		// �� ������ ������� ������ ��鿡 �����ϴ� ���� ���ϴ� �Լ� ��ü (���� ��鿡 ����)
		std::function<Vertex3D(const Vertex3D& startVertex, const Vertex3D& endVertex)> GetEdgeVertexFunc;
		// ���� ����� �����ϴ� ���� (true�� ��� ���� �ܺο� �ִ� ��)
		std::array<bool, 3> TestResult;
	};

	// Ŭ�� ��ǥ���� ������� ���̰��� W���� 0���� ������ ī�޶� �ڿ� �ִٴ� �ǹ̷� ������ ī�޶󺸴� �ڿ� �ִ��� �Ǻ�
	// (Ŭ�� ��ǥ�� ������ ī�޶��� ��ġ)
	static std::function<bool(Vertex3D)> TestFuncW0 = [](const Vertex3D& vertex) {
		// ī�޶󺸴� �ڿ� �ִ��� �Ǻ�
		return vertex.Pos.W < 0.f;
	};

	// w���� 0�϶��� ���ο� �� ���ϱ�, �� ī�޶� �ڿ� �ִ� ������ ī�޶� �ִ� w = 0�� ��鿡 ����
	static std::function<Vertex3D(Vertex3D, Vertex3D)> EdgeFuncW0 = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W;
		float p2 = endVertex.Pos.W;
		float t = p1 / (p1 - p2);
		// ���� ������ ������ ������ P3 = (1.f - t) * P1 + t * P2;
		return startVertex * (1.f - t) + endVertex * t;
	};

	// ������ ����ü ����� ����� �ܺο� �ִ��� �Ǻ�
	static auto TestFuncPY = [](const Vertex3D& vertex) {
		return vertex.Pos.Y > vertex.Pos.W;
		};

	// ������ ����ü ����� ��鿡 ����
	static auto EdgeFuncPY = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W - startVertex.Pos.Y;
		float p2 = endVertex.Pos.W - endVertex.Pos.Y;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// ������ ����ü�� �ϴ� ��麸�� �Ʒ��� �ִ��� �Ǻ� (��, �ܺο� �ִ��� �Ǻ�)
	static auto TestFuncNY = [](const Vertex3D& vertex) {
		return vertex.Pos.Y < -vertex.Pos.W;
	};

	// ����ü �ϴ��� ��鿡 ������ ����
	static auto EdgeFuncNY = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W + startVertex.Pos.Y;
		float p2 = endVertex.Pos.W + endVertex.Pos.Y;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// ������ ����ü ������ ����� �ܺο� �ִ��� �Ǻ�
	static auto TestFuncPX = [](const Vertex3D& vertex) {
		return vertex.Pos.X > vertex.Pos.W;
	};

	// ������ ����ü ���� ��鿡 ����
	static auto EdgeFuncPX = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W - startVertex.Pos.X;
		float p2 = endVertex.Pos.W - endVertex.Pos.X;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// ������ ����ü ������ ����� �ܺο� �ִ��� �Ǻ�
	static auto TestFuncNX = [](const Vertex3D& vertex) {
		return vertex.Pos.X < -vertex.Pos.W;
	};

	// ������ ����ü ���� ��鿡 ����
	static auto EdgeFuncNX = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W + startVertex.Pos.X;
		float p2 = endVertex.Pos.W + endVertex.Pos.X;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// ������ ����ü�� ������� �ܺο� �ִ��� �Ǻ�
	static auto TestFuncNear = [](const Vertex3D& vertex) {
		return vertex.Pos.Z < -vertex.Pos.W;
	};

	// ������ ����ü ����鿡 ����
	static auto EdgeFuncNear = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W + startVertex.Pos.Z;
		float p2 = endVertex.Pos.W + endVertex.Pos.Z;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};

	// ������ ����ü�� ������� �ܺο� �ִ��� �Ǻ�
	static auto TestFuncFar = [](const Vertex3D& vertex) {
		return vertex.Pos.Z > vertex.Pos.W;
	};

	// ������ ����ü�� ����鿡 ����
	static auto EdgeFuncFar = [](const Vertex3D& startVertex, const Vertex3D& endVertex) {
		float p1 = startVertex.Pos.W - startVertex.Pos.Z;
		float p2 = endVertex.Pos.W - endVertex.Pos.Z;
		float t = p1 / (p1 - p2);
		return startVertex * (1.f - t) + endVertex * t;
	};
}