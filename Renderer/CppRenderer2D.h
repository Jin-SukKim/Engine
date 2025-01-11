#pragma once
#include "CppRenderer.h"

namespace JE {

class CppRenderer2D : public CppRenderer, public IRenderer2D 
{
	using Super = CppRenderer;
public:
	CppRenderer2D(HWND hwnd) : Super(hwnd) {}
	~CppRenderer2D() override {}

	// 2D Mesh 그리기
	void DrawMesh(const Mesh2D* mesh, const Matrix3x3& mat, const Texture* texture) override;

	void SetViewCamera(Camera2DComponent* cam) { _cam = cam; }
private:
	// 2D Mesh의 삼각형 그리기
	void drawTriangle(std::array<Vertex2D, 3>& triangle, const Texture* texture);
	// 최소한의 픽셀만 그려주기 위한 화면에 그려질 삼각형을 포함하는 Bounding Box 범위 구하기
	void findTriangleBoundingBox(ScreenPoint& outLowerLeft, ScreenPoint& outUpperRight, const std::array<Vertex2D, 3>& triangle);

private:
	Camera2DComponent* _cam = nullptr;
};
}

