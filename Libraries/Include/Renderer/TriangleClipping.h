#pragma once

namespace JE {
	/*
		����ü�� �°� �ﰢ���� �߶󳻴� �ﰢ�� Clipping���� �� ��龿 �׽�Ʈ�� ������ ������ ����
		�� 7���� TriangleClipping ����ü�� ������ ������ ����ü ��鿡 �׽�Ʈ�� ����
		TriangleClipping ����ü �ϳ��� ��� �ϳ��� �׽�Ʈ
	*/
	struct TriangleClipping
	{
		TriangleClipping(std::function<bool(const Vertex3D& vertex)> testFunc,
			std::function<Vertex3D(const Vertex3D& startVertex, const Vertex3D& endVertex)> edgeFunc)
			: ClippingTestFunc(testFunc), GetEdgeVertexFunc(edgeFunc), TestResult({false, false, false}) {
		}

		// �ﰢ���� Clipping�ϴ� �Լ� : Parameter�� �޴� ������ �迭�� �ﰢ���� �����ϸ� ����
		void ClipTriangle(std::vector<Vertex3D>& InOutVertices);

	private:
		// �� �ϳ��� ����� �ٱ��� �־� �ﰢ���� 2���� �ɰ����� ��� (�� ���� �� ������ ������ �� ���� �ﰢ������ �ɰ����� �簢���� ����)
		void divideIntoTwoTriangles(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount);
		// �� �� ���� ����� �ٱ��� �־� �ﰢ���� �� ���� ���ϴ� ��� (������ ��ġ�� ������ �ﰢ���� �����ϰ� �ϳ�)
		void clipTriangle(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount);
	public:
		// �ش� ���� ��� �ٱ��� �ִ��� �����ϴ� �Լ� ��ü
		std::function<bool(const Vertex3D& vertex)> ClippingTestFunc;
		// �� ������ ������� ������ ��鿡 �����ϴ� ���� ���ϴ� �Լ� ��ü (���� ��鿡 ����)
		std::function<Vertex3D(const Vertex3D& startVertex, const Vertex3D& endVertex)> GetEdgeVertexFunc;
		// ���� ����� �����ϴ� ���� (true�� ��� ���� �ܺο� �ִ� ��)
		std::array<bool, 3> TestResult;
	};

}