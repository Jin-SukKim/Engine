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
        // ����ü �ø�
        if (FrustumCulling(mat, tr->GetPos()))
            return;
        
        std::vector<Vertex3D> vertices = mesh->GetVertices();
        const std::vector<uint32>& indices = mesh->GetIndices();

        // ���� ��ȯ
        if (_cam)
            mat = mat * _cam->GetViewPerspectiveMatrix();

        VertexShader3D(vertices, mat); // mesh�� ������ finalMatrix ����

        // �ﰢ�� Ŭ����
        std::vector<PerspectiveTest> testPlanes = {
            // { ���� ��ġ �Ǻ� �Լ�, ���� ��鿡 �����ϴ� �Լ� }
            { TestFuncW0, EdgeFuncW0 }, // ī�޶� ����
            { TestFuncNX, EdgeFuncNX }, // ����ü ���� ���
            { TestFuncPX, EdgeFuncPX }, // ����ü ������ ���
            { TestFuncPY, EdgeFuncPY }, // ����ü ���� ���
            { TestFuncNY, EdgeFuncNY }, // ����ü �Ʒ��� ���
            { TestFuncNear, EdgeFuncNear }, // ����ü �����
            { TestFuncFar, EdgeFuncFar }, // ����ü �����
        };

        // ��� �ﰢ��
        for (uint32 ti = 0; ti < indices.size(); ti += 3) {
            // Mesh�� �ﰢ��
            std::vector<Vertex3D> triangle = { vertices[indices[ti]], vertices[indices[ti + 1]], vertices[indices[ti + 2]] };

            // ����ü ��鿡 ���� �ﰢ�� Clipping ����
            for (PerspectiveTest& p : testPlanes)
                p.ClipTriangle(triangle);

            // �ﰢ�� Clipping���� ���ҵ� �ﰢ������ �߰��� �ﰢ���� �׸���
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

        // ���� ���� �����ͷ� ����ü �ø�
        _cam->CreateFrustum(mat);

        // ����ü �ø��� ���� ����ü
        Frustum frustum = _cam->GetFrustum();
        // ����ü �ٱ��� �ִٸ� �׸��� �ʰ� �ǳʶٱ�
        if (frustum.CheckBound(pos) == BoundCheckResult::Outside)
            return true;
        return false;
    }

    void CppRenderer3D::drawTriangle(std::vector<Vertex3D>& triangle, const Texture* texture, const Vector3& viewDir)
    {
        // ���̰��� ���� NDC ��ǥ�� ��ȯ (����)
        for (Vertex3D& v : triangle) {
            // Ŭ�� ��ǥ�� NDC ��ǥ�� ��ȯ
            // ������ �ſ� ����� ��� ���� (0���� ���� �� ���� ����)
            float depth = v.Pos.W; // Ŭ�� ��ǥ�� W���� �� ������ ���̰�
            if (depth == 0.f)
                depth = SMALL_NUMBER; // ��꿡 ������ ������ ���� ���� �� ���

            float invDepth = 1.f / depth;
            // �� ������ ���̰����� ���� ���̰��� ������ NDC ��ǥ�� ��ȯ
            // NDC ��ǥ�� ���� : X�� = [-1, 1], Y�� = [-1, 1], Z�� = [0, 1]
            v.Pos.X *= invDepth;
            v.Pos.Y *= invDepth;
            v.Pos.Z *= invDepth;
            // �� ������ ���̰��� W���� ���� ���� ��Ī�� ���� ���
        }

        // ������ �ʴ� �޸� �׸��⸦ ������ ������ ��귮 ���̱�
        if (_useBackfaceCulling) {
            Vector3 edge1 = (triangle[1].Pos - triangle[0].Pos).ToVector3();
            Vector3 edge2 = (triangle[2].Pos - triangle[0].Pos).ToVector3();
            Vector3 faceNormal = edge1.Cross(edge2);

            // �ﰢ���� �ո��� Normal Vector�� �þ� ������ ���̰��� 90�� �̻��� ��� ������ �����Ƿ� �׸��� ����
            if (faceNormal.Dot(viewDir) >= 0.f)
                return;
        }

        // NDC ��ǥ�� ȭ�� ��ǥ�迡 �°� �ø���
        // NDC ��ǥ ���� [-1, 1] x [-1, 1]�� ȭ�� ��ǥ�� ũ�⿡ �°� �÷��ֱ� (ȭ�� ũ���� ���� ���ϱ�)
        for (Vertex3D& v : triangle) {
            v.Pos.X *= _screenSize.X * 0.5f;
            v.Pos.Y *= _screenSize.Y * 0.5f;
            // Z ���� ���̰����� ��� [0, 1] ���� ����
        }

        if (IsWireframe()) {
            drawLine(triangle[0].Pos, triangle[1].Pos, triangle[0].Color);
            drawLine(triangle[0].Pos, triangle[2].Pos, triangle[1].Color);
            drawLine(triangle[1].Pos, triangle[2].Pos, triangle[2].Color);
            return;
        }

        // �ﰢ���� �����ϴ� Bounding Box ����
        ScreenPoint lowerLeft, upperRight;
        findTriangleBoundingBox(lowerLeft, upperRight, triangle);

        // �����߽���ǥ�� ���� �ﰢ���� ����
        Vector2 u = (triangle[1].Pos - triangle[0].Pos).ToVector2();
        Vector2 v = (triangle[2].Pos - triangle[0].Pos).ToVector2();

        // �����߽���ǥ�� ����и� (uv * uv - uu * vv)
        float udotv = u.Dot(v);
        float udotu = u.Dot(u);
        float vdotv = v.Dot(v);
        float denominator = udotv * udotv - udotu * vdotv;

        // ��ȭ�ﰢ���� �׸��� ���� (���� u, v�� ũ�Ⱑ 0�̰ų� �� ���Ͱ� �̷�� ���� 0��, 180�� �϶� �� ���Ͱ� �����ϸ� �ﰢ���� �ƴ� ������ �������)
        if (denominator == 0.f)
            return;

        denominator = 1.f / denominator; // �и�

        // �� �������� ������ ���� ���� ��Ī�� ���� ������ 
        // (VertexShader()���� �� �������� ��ȯ�Ǹ� W�� �� ������ ���̰��� ����Ǿ� ����)
        float invZ0 = 1.f / triangle[0].Pos.W;
        float invZ1 = 1.f / triangle[1].Pos.W;
        float invZ2 = 1.f / triangle[2].Pos.W;

        // Bounding Box���� Pixel��
        for (int y = upperRight.Y; y <= lowerLeft.Y; ++y) {
            for (int x = lowerLeft.X; x <= upperRight.X; ++x) {
                ScreenPoint pixel = ScreenPoint(x, y);
                Vector2 pixelPos = pixel.ToCartesianCoordinate(_screenSize);

                // �����߽���ǥ�� ���� ���� W
                Vector2 w = pixelPos - triangle[0].Pos.ToVector2();
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

                // ���� ���� ���� ��Ī�� ����� ����и�
                float z = invZ0 * oneMinusST + invZ1 * s + invZ2 * t;
                float invZ = 1.f / z;

                // Depth Testing
                // �����߽���ǥ�� ����� ���� ���� ���� ���
                float newDepth = triangle[0].Pos.Z * oneMinusST + triangle[1].Pos.Z * s + triangle[2].Pos.Z * t;
                float prevDepth = GetDepthBufferValue(pixel);

                // ���� ���ο� Pixel�� �� �տ� �ִٸ�
                if (newDepth < prevDepth)
                    SetDepthBufferValue(pixel, newDepth);
                // �̹� �տ� ���� �׷��� �����Ƿ� ����
                else
                    continue;

                // ���� ���� �ð�ȭ�� �׸��� ���
                if (IsDepthBufferDraw()) {
                    // ī�޶�κ����� �Ÿ��� ���� �����ϰ� �����ϴ� ��� ������ ��ȯ
                    float grayScale = (invZ - _nearZ) / (_farZ - _nearZ); // �������� ���������� ����
                    DrawPoint(pixel, Color::White * grayScale);
                }
                else {
                    Color color;
                    if (texture) {
                        // �����߽���ǥ�� Ȱ���� �ȼ��� uv ��ǥ
                        Vector2 uv = (triangle[0].UV * oneMinusST * invZ0 + triangle[1].UV * s * invZ1 + triangle[2].UV * t * invZ2) * invZ;
                        color = texture->SamplePoint(uv);
                    }
                    else
                        // �����߽���ǥ�� Ȱ���� �ﰢ���� ������ ������ �ȼ��� �� ����
                        color = triangle[0].Color * oneMinusST + triangle[1].Color * s + triangle[2].Color * t;

                    PixelShader(color, Color::White);
                    DrawPoint(pixel, color);
                }
            }
        }

    }

    void CppRenderer3D::findTriangleBoundingBox(ScreenPoint& outLowerLeft, ScreenPoint& outUpperRight, const std::vector<Vertex3D>& triangle)
    {
        // �ﰢ���� ���� ���� ��ǥ�� ū ��ǥ
        Vector2 minPos = Vector2(
            std::min({ triangle[0].Pos.X, triangle[1].Pos.X, triangle[2].Pos.X }),
            std::min({ triangle[0].Pos.Y, triangle[1].Pos.Y, triangle[2].Pos.Y }));
        Vector2 maxPos = Vector2(
            std::max({ triangle[0].Pos.X, triangle[1].Pos.X, triangle[2].Pos.X }),
            std::max({ triangle[0].Pos.Y, triangle[1].Pos.Y, triangle[2].Pos.Y }));

        // ȭ����� Bounding Box ����
        outLowerLeft = ScreenPoint::ToScreenCoordinate(_screenSize, minPos);
        outUpperRight = ScreenPoint::ToScreenCoordinate(_screenSize, maxPos);

        // �� ���� ȭ�� ���� ����� ��� Clipping ó��
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