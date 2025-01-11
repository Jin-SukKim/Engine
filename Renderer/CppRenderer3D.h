#pragma once
#include "CppRenderer.h"

namespace JE {
	class CameraComponent;
	class CppRenderer3D : public CppRenderer, IRenderer3D
    {
        using Super = CppRenderer;
    public:
        CppRenderer3D(HWND hwnd) : Super(hwnd) {}
        ~CppRenderer3D() override {}

		// Render ��� ���� : Normal, Wireframe etc
		bool IsDepthBufferDraw() const override { return _mode == DrawMode::DepthBuffer; };
		void SetBackfaceCulling(bool culling) override { _useBackfaceCulling = culling; }
		bool IsBackfaceCulling() const override { return _useBackfaceCulling; }

		// ���� ����
		float GetDepthBufferValue(const ScreenPoint& pos) const override;
		void SetDepthBufferValue(const ScreenPoint& pos, float depth) override;

		// 3D Mesh �׸���
		void DrawMesh(const Mesh* mesh, const Matrix4x4& mat, const Texture* texture, const Vector3& viewDir = Vector3::UnitZ) override;

		// farPlane�� nearPlane�� �Ÿ� ����
		void SetViewPlane(float nearZ, float farZ) override { _nearZ = nearZ; _farZ = farZ; }

		bool FrustumCulling(const Matrix4x4& mat, const Vector3& pos) override;
		void SetViewCamera(CameraComponent* cam) { _cam = cam; }
    private:
		// 3D Mesh�� �ﰢ�� �׸���
        void drawTriangle(std::vector<Vertex3D>& triangle, const Texture* texture, const Vector3& viewDir);
		// �ּ����� �ȼ��� �׷��ֱ� ���� ȭ�鿡 �׷��� �ﰢ���� �����ϴ� Bounding Box ���� ���ϱ�
		void findTriangleBoundingBox(ScreenPoint& outLowerLeft, ScreenPoint& outUpperRight, const std::vector<Vertex3D>& triangle);
		// NCD ��ǥ�迡�� ȭ�� ��ǥ��� ��ȯ�� Line �׸���
		void drawLine(const Vector4& start, const Vector4& end, const Color& color);
		// NCD ��ǥ�迡�� ȭ�� ��ǥ��� ��ȯ�� Point �׸���
		void drawPoint(const Vector4& point, const Color& color);

	private:
		bool _useBackfaceCulling = true;
		float _nearZ = 10.f, _farZ = 1000.f;
		CameraComponent* _cam = nullptr;
    };

}

