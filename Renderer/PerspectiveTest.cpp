#include "pch.h"
#include "PerspectiveTest.h"

namespace JE {
	void PerspectiveTest::ClipTriangle(std::vector<Vertex3D>& InOutVertices)
	{
		// ��ü �ﰢ���� ��
		size_t triangles = InOutVertices.size() / 3;
		// �ﰢ���� ����ŭ ���� ����
		for (size_t ti = 0; ti < triangles; ++ti) {
			size_t startIndex = ti * 3; // �ﰢ���� Index
			size_t nonPassCount = 0; // ��� �ٱ��� ��ġ�� ���� ���� �����ϴ� ����

			// �ﰢ���� �� ���� ���鼭 ����ü ����� �׽�Ʈ ����
			for (size_t i = 0; i < 3; ++i) {
				// �׽�Ʈ ����� ����
				TestResult[i] = ClippingTestFunc(InOutVertices[startIndex + i]);
				if (TestResult[i]) // ���� ����� �ٱ��� ��ġ�� ���
					++nonPassCount;
			}

			// �ﰢ���� �� ������ ��� ����ü �ȿ� �ﰢ�� Ŭ���� �ǳʶٱ�
			if (nonPassCount == 0)
				continue;
			// �� ������ ����� �ٱ��� �ִ� ��� �ﰢ���� �� ���� �ɰ���
			else if (nonPassCount == 1)
				divideIntoTwoTriangles(InOutVertices, startIndex, nonPassCount);
			// �� ������ ����� �ٱ��� �ִ� ��� �ﰢ���� ���� ���� ����
			else if (nonPassCount == 2)
				clipTriangle(InOutVertices, startIndex, nonPassCount);
			// �� ������ ��� �ٱ��� ��ġ�ϸ� �ﰢ���� ������ ����
			else {
				// �ﰢ�� �����ϰ� ���ŵ��� ���� �ﰢ���� ���� ������
				InOutVertices.erase(InOutVertices.begin() + startIndex, InOutVertices.begin() + startIndex + 3);
				--triangles;
				--ti;
			}
		}
	}

	void PerspectiveTest::divideIntoTwoTriangles(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount)
	{
		// ����� �ٱ��� ��ġ�� ���� index ã��
		uint8 index = 0;
		// �ﰢ���� ù��° ������ �ƴ϶��
		if (!TestResult[0])
			index = TestResult[1] ? 1 : 2; // 2��° or 3��°

		// ����� ���ʿ� ��ġ�� �ٸ� �� ���� index
		size_t v1Index = startIndex + (index + 1) % 3;
		size_t v2Index = startIndex + (index + 2) % 3;

		// ���ʿ� ��ġ�� �ٸ� �� ����
		Vertex3D v1 = vertices[v1Index];
		Vertex3D v2 = vertices[v2Index];

		// �ٱ��� ��ġ�� ���� ��鿡 ������Ų ���ο� �� ����, �� �� ������ ������ ��������
		Vertex3D clipped1 = GetEdgeVertexFunc(vertices[startIndex + index], v1);
		Vertex3D clipped2 = GetEdgeVertexFunc(vertices[startIndex + index], v2);

		// ��������(���ο� �� ����)�� ����� �ﰢ���� �� ���� ���� -> �ﰢ�� 2���� �̷��� �簢���� ���°� �� 
		// �ﰢ�� 1
		vertices[startIndex] = clipped1;
		vertices[startIndex + 1] = v1;
		vertices[startIndex + 2] = v2;
		// �ﰢ�� 2
		vertices.emplace_back(clipped1);
		vertices.emplace_back(v2);
		vertices.emplace_back(clipped2);
	}

	void PerspectiveTest::clipTriangle(std::vector<Vertex3D>& vertices, size_t startIndex, size_t nonPassCount)
	{
		// ����� ���ʿ� ��ġ�� ���� index ã�� (�� ���� ������ �ٱ��� ��ġ)
		uint8 index = 0;
		if (TestResult[0])
			index = !TestResult[1] ? 1 : 2;

		// ����� �ٱ��� ��ġ�� �� ������ index
		size_t v1Index = startIndex + (index + 1) % 3;
		size_t v2Index = startIndex + (index + 2) % 3;

		// ����� �ٱ��� ��ġ�� �� ����
		Vertex3D v1 = vertices[v1Index];
		Vertex3D v2 = vertices[v2Index];

		// �ٱ��� ��ġ�� �� ���� ��鿡 ����, �� �� ������ ������ ��������
		Vertex3D clipped1 = GetEdgeVertexFunc(vertices[startIndex + index], v1);
		Vertex3D clipped2 = GetEdgeVertexFunc(vertices[startIndex + index], v2);

		// �ٱ��� ��ġ�� �� ���� ����ü ��� ������ �̵�
		vertices[v1Index] = clipped1;
		vertices[v2Index] = clipped2;
	}
}