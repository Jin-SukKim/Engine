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

		// Render 모드 설정 : Normal, Wireframe etc
		bool IsDepthBufferDraw() const override { return _mode == DrawMode::DepthBuffer; };
		void SetBackfaceCulling(bool culling) override { _useBackfaceCulling = culling; }
		bool IsBackfaceCulling() const override { return _useBackfaceCulling; }

		// 깊이 버퍼
		float GetDepthBufferValue(const ScreenPoint& pos) const override;
		void SetDepthBufferValue(const ScreenPoint& pos, float depth) override;

		// 3D Mesh 그리기
		void DrawMesh(const Mesh* mesh, const Matrix4x4& mat, const Texture* texture, const Vector3& viewDir = Vector3::UnitZ) override;

		// farPlane과 nearPlane의 거리 설정
		void SetViewPlane(float nearZ, float farZ) override { _nearZ = nearZ; _farZ = farZ; }

		bool FrustumCulling(const Matrix4x4& mat, const Vector3& pos) override;
		void SetViewCamera(CameraComponent* cam) { _cam = cam; }
    private:
		// 3D Mesh의 삼각형 그리기
        void drawTriangle(std::vector<Vertex3D>& triangle, const Texture* texture, const Vector3& viewDir);
		// 최소한의 픽셀만 그려주기 위한 화면에 그려질 삼각형을 포함하는 Bounding Box 범위 구하기
		void findTriangleBoundingBox(ScreenPoint& outLowerLeft, ScreenPoint& outUpperRight, const std::vector<Vertex3D>& triangle);
		// NCD 좌표계에서 화면 좌표계로 변환된 Line 그리기
		void drawLine(const Vector4& start, const Vector4& end, const Color& color);
		// NCD 좌표계에서 화면 좌표계로 변환된 Point 그리기
		void drawPoint(const Vector4& point, const Color& color);

	private:
		bool _useBackfaceCulling = true;
		float _nearZ = 10.f, _farZ = 1000.f;
		CameraComponent* _cam = nullptr;
    };

}

