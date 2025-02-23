#include "pch.h"
#include "Frustum.h"

namespace JE {
BoundCheckResult Frustum::CheckBound(const Vector3& point) const
{
    // ����ü�� �����ϴ� 6���� ��鿡 ���� �׽�Ʈ ����
    for (const Plane& p : Planes) {
        // �� ����̶� ���� �ٱ��� ������ ���� �ܺο� ������ �Ǻ��ϰ� ����
        if (p.IsOutside(point))
            return BoundCheckResult::Outside;
        // ���� ���� ���� �Ÿ��� 0�� �����ϸ�, ��ġ�� ������ �Ǻ��ϰ� ���� (plane�� ����� �Ÿ��� 0�� ������� Ȯ��)
        else if (MathUtils::EqualsInTolerance(p.Distance(point), 0.f)) 
            return BoundCheckResult::Intersect;
    }

    // ��� ��鿡 ���� �׽�Ʈ�� ����ϸ� �������� �Ǻ�
    return BoundCheckResult::Inside;
}

BoundCheckResult Frustum::CheckBound(const Sphere& sphere) const
{
    // ����ü�� �����ϴ� 6���� ��鿡 ���� �׽�Ʈ ����
    for (const Plane& p : Planes) {
        // ���� ����� �ٱ��� ���� �� ���� ����� �Ÿ��� ���� ���������� ũ�ٸ� ������ �ٱ��� �ִٰ� �Ǵ�
        if (p.Distance(sphere.Center) > sphere.Radius) // radius�� ����̹Ƿ� Distance�� ������ ��� ���õǰ�, distance�� 0���� �۴ٸ� ����� �ٱ��� �ƴ϶� �������.
            return BoundCheckResult::Outside;
        // ���� ����� �Ÿ��� ���� ���������� �۰ų� ������ ��鿡 �����ִٰ� �Ǵ�
        else if (std::abs(p.Distance(sphere.Center)) <= sphere.Radius)
            return BoundCheckResult::Intersect;
    }

    // ��� ��鿡 ���� �׽�Ʈ�� ����ϸ� �������� �Ǵ�
    return BoundCheckResult::Inside;
}

BoundCheckResult Frustum::CheckBound(const Box& box) const
{
    // ����ü�� �����ϴ� 6���� ��鿡 ���� �׽�Ʈ ����
    for (const Plane& p : Planes) {
        /*
            ����� ���� ����(Normal Vector)�� ��� ��Ұ� ���(+, +, +)�� ��Ȳ���� AABB ������ �ٱ��� �ִٸ�, ��鿡 ���� ����� ���� �ּڰ��� �ȴ�.
            �̶� �ּڰ��� ����� D(�Ÿ�)���� ����� ���´�.

            ���� ������ ��� ��Ұ� ����(-, -, -)�� ��Ȳ���� AABB ������ �ٱ��� �ִٸ�, ��鿡�� ���� ����� ���� �ִ밪�� �ȴ�.
            �̶� �ִ񰪰� ����� D(�Ÿ�)�� ���� ����� ���´�.

            AABB ������ ���� ������ X, Y, Z���� ���� �����ϹǷ� �� ���� �����ʹ� ���������� �����Ѵ�.
            ���� �� ���� ������ ��Ҹ��� ���(+)�� ��� �ּڰ���, ����(-)�� ��� �ִ밪�� ����ϸ� ���� ���� ����� AABB�� ���� ���� �� �ִ�

            ex) ���� ���� = (+, -, +)��� ���� ���� ����� AABB�� �� = (�ּڰ�.X, �ִ�.Y, �ּڰ�.Z)�� �ȴ�.
        */

        // �ڽ�(AABB) �������� �׽�Ʈ�� �� ��
        // ���� ���� ����� AABB ������ �� (���� Normal Vector�� (+, +, +)�� ���� �ʱ�ȭ
        Vector3 near = box.Min;
        // ���� ���� ����� AABB ������ ���� ���ݴ뿡 ��ġ�� �� (�ݴ� �밢�� ��ġ)
        Vector3 opposite = box.Max;

        // ����� Normal Vector ���� ��ȣ�� �ľ��� �� ���� ���� ����
        if (p.Normal.X < 0.f) {
            near.X = box.Max.X;
            opposite.X = box.Min.X;
        }
        if (p.Normal.Y < 0.f) {
            near.Y = box.Max.Y;
            opposite.Y = box.Min.Y;
        }
        if (p.Normal.Z < 0.f) {
            near.Z = box.Max.Z;
            opposite.Z = box.Min.Z;
        }

        // ���� AABB�� ���� ����� ���� �Ÿ� �׽�Ʈ ���� ������ ����� �ܺ�
        if (p.Distance(near) > 0.f)
            return BoundCheckResult::Outside;
        // �� ���� ���̿� ����� �ִٸ� �ڽ��� ����� ��ġ�� �� (���� AABB�� ���� ����� ���� �Ÿ��� ������ ������(����� ���ο� ����� ��), ���ݴ� ��ġ�� ������ �Ÿ��� ����� ������(����� �ܺ�))
        if (p.Distance(near) <= 0.f && p.Distance(opposite) >= 0.f) 
            return BoundCheckResult::Intersect;
    }

    // ��� ��鿡 ���� �׽�Ʈ�� ����ϸ� �������� �Ǵ�
    return BoundCheckResult::Inside;
}
}