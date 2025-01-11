#pragma once
#include "CppRenderer.h"

namespace JE {

class CppRenderer2D : public CppRenderer, public IRenderer2D 
{
	using Super = CppRenderer;
public:
	CppRenderer2D(HWND hwnd) : Super(hwnd) {}
	~CppRenderer2D() override {}

	// 2D Mesh �׸���
	void DrawMesh(const Mesh2D* mesh, const Matrix3x3& mat, const Texture* texture) override;

	void SetViewCamera(Camera2DComponent* cam) { _cam = cam; }
private:
	// 2D Mesh�� �ﰢ�� �׸���
	void drawTriangle(std::array<Vertex2D, 3>& triangle, const Texture* texture);
	// �ּ����� �ȼ��� �׷��ֱ� ���� ȭ�鿡 �׷��� �ﰢ���� �����ϴ� Bounding Box ���� ���ϱ�
	void findTriangleBoundingBox(ScreenPoint& outLowerLeft, ScreenPoint& outUpperRight, const std::array<Vertex2D, 3>& triangle);

private:
	Camera2DComponent* _cam = nullptr;
};
}

