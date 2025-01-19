#include "pch.h"
#include "CppRenderer3D.h"
#include "PerspectiveTest.h"
#include "Engine/CameraComponent.h"

namespace JE {
    void CppRenderer3D::SetDepthBufferValue(const ScreenPoint& pos, float depth)
    {
        Super::SetDepthBufferValue(pos, depth);
    }

    float CppRenderer3D::GetDepthBufferValue(const ScreenPoint& pos) const
    {
        return Super::GetDepthBufferValue(pos);
    }

    void CppRenderer3D::DrawMesh(const Mesh* mesh, const TransformComponent* tr, const Texture* texture, const Vector3& viewDir)
    {
        Matrix4x4 mat = tr->GetTransformMatrix();
        // 절두체 컬링
        if (FrustumCulling(mat, tr->GetPos()))
            return;
        
        std::vector<Vertex3D> vertices = mesh->GetVertices();
        const std::vector<uint32>& indices = mesh->GetIndices();

        // 정점 변환
        if (_cam)
            mat = mat * _cam->GetViewPerspectiveMatrix();

        VertexShader3D(vertices, mat); // mesh의 정점에 finalMatrix 적용

        // 삼각형 클리핑
        std::vector<PerspectiveTest> testPlanes = {
            // { 점의 위치 판별 함수, 점을 평면에 투영하는 함수 }
            { TestFuncW0, EdgeFuncW0 }, // 카메라 원점
            { TestFuncNX, EdgeFuncNX }, // 절두체 왼쪽 평면
            { TestFuncPX, EdgeFuncPX }, // 절두체 오른쪽 평면
            { TestFuncPY, EdgeFuncPY }, // 절두체 위쪽 평면
            { TestFuncNY, EdgeFuncNY }, // 절두체 아래쪽 평면
            { TestFuncNear, EdgeFuncNear }, // 절두체 근평면
            { TestFuncFar, EdgeFuncFar }, // 절두체 원평면
        };

        // 모든 삼각형
        for (uint32 ti = 0; ti < indices.size(); ti += 3) {
            // Mesh의 삼각형
            std::vector<Vertex3D> triangle = { vertices[indices[ti]], vertices[indices[ti + 1]], vertices[indices[ti + 2]] };

            // 절두체 평면에 대해 삼각형 Clipping 진행
            for (PerspectiveTest& p : testPlanes)
                p.ClipTriangle(triangle);

            // 삼각형 Clipping으로 분할된 삼각형까지 추가된 삼각형들 그리기
            for (uint32 i = 0; i < triangle.size(); i += 3) {
                std::vector<Vertex3D> sub(triangle.begin() + i, triangle.begin() + i + 3);
                drawTriangle(sub, texture, viewDir);
            }
        }
    }

    bool CppRenderer3D::FrustumCulling(const Matrix4x4& mat, const Vector3& pos)
    {
        if (_cam == nullptr)
            return false;

        // 로컬 공간 데이터로 절두체 컬링
        _cam->CreateFrustum(mat);

        // 절두체 컬링을 위한 절두체
        Frustum frustum = _cam->GetFrustum();
        // 절두체 바깥에 있다면 그리지 않고 건너뛰기
        if (frustum.CheckBound(pos) == BoundCheckResult::Outside)
            return true;
        return false;
    }

    void CppRenderer3D::drawTriangle(std::vector<Vertex3D>& triangle, const Texture* texture, const Vector3& viewDir)
    {
        // 깊이값을 가진 NDC 좌표로 변환 (투영)
        for (Vertex3D& v : triangle) {
            // 클릭 좌표를 NDC 좌표로 변환
            // 원점에 매우 가까운 경우 보정 (0으로 나눌 수 없기 때문)
            float depth = v.Pos.W; // 클립 좌표의 W값은 뷰 공간의 깊이값
            if (depth == 0.f)
                depth = SMALL_NUMBER; // 계산에 지장이 없도록 아주 작은 값 사용

            float invDepth = 1.f / depth;
            // 뷰 공간의 깊이값으로 나눠 깊이값을 가지는 NDC 좌표로 변환
            // NDC 좌표계 범위 : X축 = [-1, 1], Y축 = [-1, 1], Z축 = [0, 1]
            v.Pos.X *= invDepth;
            v.Pos.Y *= invDepth;
            v.Pos.Z *= invDepth;
            // 뷰 공간의 깊이값인 W값은 원근 보정 매칭을 위해 사용
        }

        // 보이지 않는 뒷면 그리기를 생략해 렌더링 계산량 줄이기
        if (_useBackfaceCulling) {
            Vector3 edge1 = (triangle[1].Pos - triangle[0].Pos).ToVector3();
            Vector3 edge2 = (triangle[2].Pos - triangle[0].Pos).ToVector3();
            Vector3 faceNormal = edge1.Cross(edge2);

            // 삼각형의 앞면의 Normal Vector와 시야 방향의 사이각이 90도 이상인 경우 보이지 않으므로 그리기 생략
            if (faceNormal.Dot(viewDir) >= 0.f)
                return;
        }

        // NDC 좌표를 화면 좌표계에 맞게 늘리기
        // NDC 좌표 범위 [-1, 1] x [-1, 1]을 화면 좌표계 크기에 맞게 늘려주기 (화면 크기의 절반 곱하기)
        for (Vertex3D& v : triangle) {
            v.Pos.X *= _screenSize.X * 0.5f;
            v.Pos.Y *= _screenSize.Y * 0.5f;
            // Z 값은 깊이값으로 사용 [0, 1] 범위 유지
        }

        if (IsWireframe()) {
            drawLine(triangle[0].Pos, triangle[1].Pos, triangle[0].Color);
            drawLine(triangle[0].Pos, triangle[2].Pos, triangle[1].Color);
            drawLine(triangle[1].Pos, triangle[2].Pos, triangle[2].Color);
            return;
        }

        // 삼각형을 포함하는 Bounding Box 영역
        ScreenPoint lowerLeft, upperRight;
        findTriangleBoundingBox(lowerLeft, upperRight, triangle);

        // 무게중심좌표를 위한 삼각형의 벡터
        Vector2 u = (triangle[1].Pos - triangle[0].Pos).ToVector2();
        Vector2 v = (triangle[2].Pos - triangle[0].Pos).ToVector2();

        // 무게중심좌표의 공통분모 (uv * uv - uu * vv)
        float udotv = u.Dot(v);
        float udotu = u.Dot(u);
        float vdotv = v.Dot(v);
        float denominator = udotv * udotv - udotu * vdotv;

        // 퇴화삼각형은 그리지 않음 (벡터 u, v의 크기가 0이거나 두 벡터가 이루는 각이 0도, 180도 일때 두 벡터가 평행하며 삼각형이 아닌 선분이 만들어짐)
        if (denominator == 0.f)
            return;

        denominator = 1.f / denominator; // 분모

        // 각 정점마다 보존되 원근 보정 매칭을 위한 보정값 
        // (VertexShader()에서 뷰 공간으로 변환되며 W에 뷰 공간의 깊이값이 저장되어 있음)
        float invZ0 = 1.f / triangle[0].Pos.W;
        float invZ1 = 1.f / triangle[1].Pos.W;
        float invZ2 = 1.f / triangle[2].Pos.W;

        // Bounding Box안의 Pixel들
        for (int y = upperRight.Y; y <= lowerLeft.Y; ++y) {
            for (int x = lowerLeft.X; x <= upperRight.X; ++x) {
                ScreenPoint pixel = ScreenPoint(x, y);
                Vector2 pixelPos = pixel.ToCartesianCoordinate(_screenSize);

                // 무게중심좌표를 위한 벡터 W
                Vector2 w = pixelPos - triangle[0].Pos.ToVector2();
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

                // 원근 투영 보정 매칭에 사용할 공통분모
                float z = invZ0 * oneMinusST + invZ1 * s + invZ2 * t;
                float invZ = 1.f / z;

                // Depth Testing
                // 무게중심좌표를 사용해 현재 점의 깊이 계산
                float newDepth = triangle[0].Pos.Z * oneMinusST + triangle[1].Pos.Z * s + triangle[2].Pos.Z * t;
                float prevDepth = GetDepthBufferValue(pixel);

                // 만약 새로운 Pixel이 더 앞에 있다면
                if (newDepth < prevDepth)
                    SetDepthBufferValue(pixel, newDepth);
                // 이미 앞에 무언가 그려져 있으므로 생략
                else
                    continue;

                // 깊이 버퍼 시각화로 그리는 모드
                if (IsDepthBufferDraw()) {
                    // 카메라로부터의 거리에 따라 균일하게 증감하는 흑백 값으로 변환
                    float grayScale = (invZ - _nearZ) / (_farZ - _nearZ); // 근평면부터 원평면까지의 범위
                    DrawPoint(pixel, Color::White * grayScale);
                }
                else {
                    Color color;
                    if (texture) {
                        // 무게중심좌표를 활용한 픽셀의 uv 좌표
                        Vector2 uv = (triangle[0].UV * oneMinusST * invZ0 + triangle[1].UV * s * invZ1 + triangle[2].UV * t * invZ2) * invZ;
                        color = texture->SamplePoint(uv);
                    }
                    else
                        // 무게중심좌표를 활용해 삼각형의 정점의 색으로 픽셀의 색 지정
                        color = triangle[0].Color * oneMinusST + triangle[1].Color * s + triangle[2].Color * t;

                    PixelShader(color, Color::White);
                    DrawPoint(pixel, color);
                }
            }
        }

    }

    void CppRenderer3D::findTriangleBoundingBox(ScreenPoint& outLowerLeft, ScreenPoint& outUpperRight, const std::vector<Vertex3D>& triangle)
    {
        // 삼각형의 가장 작은 좌표와 큰 좌표
        Vector2 minPos = Vector2(
            std::min({ triangle[0].Pos.X, triangle[1].Pos.X, triangle[2].Pos.X }),
            std::min({ triangle[0].Pos.Y, triangle[1].Pos.Y, triangle[2].Pos.Y }));
        Vector2 maxPos = Vector2(
            std::max({ triangle[0].Pos.X, triangle[1].Pos.X, triangle[2].Pos.X }),
            std::max({ triangle[0].Pos.Y, triangle[1].Pos.Y, triangle[2].Pos.Y }));

        // 화면상의 Bounding Box 영역
        outLowerLeft = ScreenPoint::ToScreenCoordinate(_screenSize, minPos);
        outUpperRight = ScreenPoint::ToScreenCoordinate(_screenSize, maxPos);

        // 두 점이 화면 밖을 벗어나는 경우 Clipping 처리
        outLowerLeft.X = std::max(outLowerLeft.X, 0);
        outLowerLeft.Y = std::min(outLowerLeft.Y, _screenSize.Y);
        outUpperRight.X = std::min(outUpperRight.X, _screenSize.X);
        outUpperRight.Y = std::max(outUpperRight.Y, 0);
    }

    void CppRenderer3D::drawLine(const Vector4& start, const Vector4& end, const Color& color)
    {
        Super::DrawLine(start.ToVector2(), end.ToVector2(), color);
    }

    void CppRenderer3D::drawPoint(const Vector4& point, const Color& color)
    {
        Super::DrawPoint(point.ToVector2(), color);
    }



}