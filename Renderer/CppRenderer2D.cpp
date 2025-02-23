#include "pch.h"
#include "CppRenderer2D.h"
#include "Engine/Camera2DComponent.h"
#include "Engine/Transform2DComponent.h"

namespace JE {
	void CppRenderer2D::DrawMesh(const Mesh2D* mesh, const Transform2DComponent* tr, const Texture* texture)
	{
		std::vector<Vertex2D> vertices = mesh->GetVertices();
		const std::vector<uint32>& indices = mesh->GetIndices();

		// ���� ��ȯ
		Matrix3x3 finalMat = tr->GetTransformMatrix();
		if (_cam)
			finalMat = finalMat * _cam->GetViewMatrix();

		VertexShader2D(vertices, finalMat); // mesh�� ������ finalMatrix ����
			

		// �ﰢ���� ����ŭ �ݺ�
		for (uint32 ti = 0; ti < indices.size(); ti += 3) {
			std::array<Vertex2D, 3> triangle = { vertices[indices[ti]], vertices[indices[ti + 1]], vertices[indices[ti + 2]] };
			// �ȼ�ȭ
			drawTriangle(triangle, texture);
		}
	}

	void CppRenderer2D::drawTriangle(std::array<Vertex2D, 3>& triangle, const Texture* texture)
	{
		// Wireframe �׸��� ���
		if (IsWireframe()) {
			DrawLine(triangle[0].Pos, triangle[1].Pos, triangle[0].Color);
			DrawLine(triangle[0].Pos, triangle[2].Pos, triangle[2].Color);
			DrawLine(triangle[1].Pos, triangle[2].Pos, triangle[1].Color);
			return;
		}
		
		// ȭ���� ��� �ȼ��� �˻��ϱ� ���ٴ� �ﰢ���� �����ϴ� Bounding Box�� ���� �� ���� Pixel�� �˻�
		ScreenPoint lowerLeft, upperRight;
		findTriangleBoundingBox(lowerLeft, upperRight, triangle);

		// �����߽���ǥ�� ���� �ﰢ���� ����
		Vector2 u = triangle[1].Pos - triangle[0].Pos;
		Vector2 v = triangle[2].Pos - triangle[0].Pos;
		
		// �����߽���ǥ�� ����и� (uv * uv - uu * vv)
		float udotv = u.Dot(v);
		float udotu = u.Dot(u);
		float vdotv = v.Dot(v);
		float denominator = udotv * udotv - udotu * vdotv;

		// ��ȭ�ﰢ���� �׸��� ���� (���� u, v�� ũ�Ⱑ 0�̰ų� �� ���Ͱ� �̷�� ���� 0��, 180�� �϶� �� ���Ͱ� �����ϸ� �ﰢ���� �ƴ� ������ �������)
		if (denominator == 0.f)
			return;

		denominator = 1.f / denominator; // �и�

		// Bounding Box���� Pixel��
		for (int y = upperRight.Y; y <= lowerLeft.Y; ++y) { // window â�� y���� ������ �Ʒ��� ���������� Ŀ��
			for (int x = lowerLeft.X; x <= upperRight.X; ++x) {
				ScreenPoint pixel = ScreenPoint(x, y);
				Vector2 pixelPos = pixel.ToCartesianCoordinate(_screenSize);

				// �����߽���ǥ�� ���� ���� w
				Vector2 w = pixelPos - triangle[0].Pos;
				float wdotu = w.Dot(u);
				float wdotv = w.Dot(v);

				// �����߽���ǥ (s, t, 1 - s - t) ���
				float s = (wdotv * udotv - wdotu * vdotv) * denominator; // ���ڰ��� ���� �и�� ������
				float t = (wdotu * udotv - wdotv * udotu) * denominator;
				float oneMinusST = 1.f - s - t;

				// �ȼ��� �ﰢ�� �ȿ� �ִٸ� ���� ��� �ﰢ�� ���̶�� ����
				// (�����߽���ǥ�� �����ϴ� ���� ��� [0, 1] ���� �ȿ� �־�� �ϰ� �ƴϸ� �ﰢ�� �ܺο� �ִ� ��)
				if (s < 0.f || s > 1.f || t < 0.f || t > 1.f || oneMinusST < 0.f || oneMinusST > 1.f)
					continue;

				Color color;
				// �����߽���ǥ�� Ȱ���� �ȼ��� uv ��ǥ�� ���� texture���� ���� ����
				if (texture) {
					Vector2 uv = triangle[0].UV * oneMinusST + triangle[1].UV * s + triangle[2].UV * t;
					color = texture->SamplePoint(uv);
				}
				// �����߽���ǥ�� Ȱ���� �ﰢ���� ������ ������ �ȼ��� �� ����
				else
					color = triangle[0].Color * oneMinusST + triangle[1].Color * s + triangle[2].Color * t;

				// ����� ���� �Ű澲�� �ʰ� ���� ������� ���� �� ǥ��
				color = PixelShader(color, Color::White);
				DrawPoint(pixel, color);
			}
		}
	}


	void CppRenderer2D::findTriangleBoundingBox(ScreenPoint& outLowerLeft, ScreenPoint& outUpperRight, const std::array<Vertex2D, 3>& triangle)
	{
		// Bounding Box�� ���� �ﰢ���� ���� ���� ��ǥ�� ū ��ǥ
		Vector2 minPos = Vector2(
			std::min({ triangle[0].Pos.X, triangle[1].Pos.X, triangle[2].Pos.X }),
			std::min({ triangle[0].Pos.Y, triangle[1].Pos.Y, triangle[2].Pos.Y }));
		Vector2 maxPos = Vector2(
			std::max({ triangle[0].Pos.X, triangle[1].Pos.X, triangle[2].Pos.X }),
			std::max({ triangle[0].Pos.Y, triangle[1].Pos.Y, triangle[2].Pos.Y }));

		// ȭ����� ���� ���ϱ�
		outLowerLeft = ScreenPoint::ToScreenCoordinate(_screenSize, minPos);
		outUpperRight = ScreenPoint::ToScreenCoordinate(_screenSize, maxPos);

		// ������ ȭ�� ���� ����� ��� Clipping ó��
		outLowerLeft.X = std::max(outLowerLeft.X, 0);
		outLowerLeft.Y = std::min(outLowerLeft.Y, _screenSize.Y);
		outUpperRight.X = std::min(outUpperRight.X, _screenSize.X);
		outUpperRight.Y = std::max(outUpperRight.Y, 0);
	}
}