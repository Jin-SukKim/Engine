#include "pch.h"
#include "CppRenderer2D.h"
#include "Engine/Camera2DComponent.h"
#include "Engine/Transform2DComponent.h"

namespace JE {
	void CppRenderer2D::DrawMesh(const Mesh2D* mesh, const Transform2DComponent* tr, const Texture* texture)
	{
		std::vector<Vertex2D> vertices = mesh->GetVertices();
		const std::vector<uint32>& indices = mesh->GetIndices();

		// 정점 변환
		Matrix3x3 finalMat = tr->GetTransformMatrix();
		if (_cam)
			finalMat = finalMat * _cam->GetViewMatrix();

		VertexShader2D(vertices, finalMat); // mesh의 정점에 finalMatrix 적용
			

		// 삼각형의 수만큼 반복
		for (uint32 ti = 0; ti < indices.size(); ti += 3) {
			std::array<Vertex2D, 3> triangle = { vertices[indices[ti]], vertices[indices[ti + 1]], vertices[indices[ti + 2]] };
			// 픽셀화
			drawTriangle(triangle, texture);
		}
	}

	void CppRenderer2D::drawTriangle(std::array<Vertex2D, 3>& triangle, const Texture* texture)
	{
		// Wireframe 그리기 모드
		if (IsWireframe()) {
			DrawLine(triangle[0].Pos, triangle[1].Pos, triangle[0].Color);
			DrawLine(triangle[0].Pos, triangle[2].Pos, triangle[2].Color);
			DrawLine(triangle[1].Pos, triangle[2].Pos, triangle[1].Color);
			return;
		}
		
		// 화면의 모든 픽셀을 검사하기 보다는 삼각형을 포함하는 Bounding Box를 구해 이 안의 Pixel만 검사
		ScreenPoint lowerLeft, upperRight;
		findTriangleBoundingBox(lowerLeft, upperRight, triangle);

		// 무게중심좌표를 위한 삼각형의 벡터
		Vector2 u = triangle[1].Pos - triangle[0].Pos;
		Vector2 v = triangle[2].Pos - triangle[0].Pos;
		
		// 무게중심좌표의 공통분모 (uv * uv - uu * vv)
		float udotv = u.Dot(v);
		float udotu = u.Dot(u);
		float vdotv = v.Dot(v);
		float denominator = udotv * udotv - udotu * vdotv;

		// 퇴화삼각형은 그리지 않음 (벡터 u, v의 크기가 0이거나 두 벡터가 이루는 각이 0도, 180도 일때 두 벡터가 평행하며 삼각형이 아닌 선분이 만들어짐)
		if (denominator == 0.f)
			return;

		denominator = 1.f / denominator; // 분모

		// Bounding Box안의 Pixel들
		for (int y = upperRight.Y; y <= lowerLeft.Y; ++y) { // window 창의 y축은 위에서 아래로 내려갈수록 커짐
			for (int x = lowerLeft.X; x <= upperRight.X; ++x) {
				ScreenPoint pixel = ScreenPoint(x, y);
				Vector2 pixelPos = pixel.ToCartesianCoordinate(_screenSize);

				// 무게중심좌표를 위한 벡터 w
				Vector2 w = pixelPos - triangle[0].Pos;
				float wdotu = w.Dot(u);
				float wdotv = w.Dot(v);

				// 무게중심좌표 (s, t, 1 - s - t) 계산
				float s = (wdotv * udotv - wdotu * vdotv) * denominator; // 분자값을 구해 분모로 나누기
				float t = (wdotu * udotv - wdotv * udotu) * denominator;
				float oneMinusST = 1.f - s - t;

				// 픽셀이 삼각형 안에 있다면 점을 찍고 삼각형 밖이라면 무시
				// (무게중심좌표를 구성하는 값이 모두 [0, 1] 범위 안에 있어야 하고 아니면 삼각형 외부에 있는 것)
				if (s < 0.f || s > 1.f || t < 0.f || t > 1.f || oneMinusST < 0.f || oneMinusST > 1.f)
					continue;

				Color color;
				// 무게중심좌표를 활용해 픽셀의 uv 좌표를 구해 texture에서 색상 지정
				if (texture) {
					Vector2 uv = triangle[0].UV * oneMinusST + triangle[1].UV * s + triangle[2].UV * t;
					color = texture->SamplePoint(uv);
				}
				// 무게중심좌표를 활용해 삼각형의 정점의 색으로 픽셀의 색 지정
				else
					color = triangle[0].Color * oneMinusST + triangle[1].Color * s + triangle[2].Color * t;

				// 현재는 빛을 신경쓰지 않고 그저 흰색으로 원래 색 표현
				color = PixelShader(color, Color::White);
				DrawPoint(pixel, color);
			}
		}
	}


	void CppRenderer2D::findTriangleBoundingBox(ScreenPoint& outLowerLeft, ScreenPoint& outUpperRight, const std::array<Vertex2D, 3>& triangle)
	{
		// Bounding Box를 위한 삼각형의 가장 작은 좌표와 큰 좌표
		Vector2 minPos = Vector2(
			std::min({ triangle[0].Pos.X, triangle[1].Pos.X, triangle[2].Pos.X }),
			std::min({ triangle[0].Pos.Y, triangle[1].Pos.Y, triangle[2].Pos.Y }));
		Vector2 maxPos = Vector2(
			std::max({ triangle[0].Pos.X, triangle[1].Pos.X, triangle[2].Pos.X }),
			std::max({ triangle[0].Pos.Y, triangle[1].Pos.Y, triangle[2].Pos.Y }));

		// 화면상의 범위 구하기
		outLowerLeft = ScreenPoint::ToScreenCoordinate(_screenSize, minPos);
		outUpperRight = ScreenPoint::ToScreenCoordinate(_screenSize, maxPos);

		// 범위가 화면 밖을 벗어나는 경우 Clipping 처리
		outLowerLeft.X = std::max(outLowerLeft.X, 0);
		outLowerLeft.Y = std::min(outLowerLeft.Y, _screenSize.Y);
		outUpperRight.X = std::min(outUpperRight.X, _screenSize.X);
		outUpperRight.Y = std::max(outUpperRight.Y, 0);
	}
}