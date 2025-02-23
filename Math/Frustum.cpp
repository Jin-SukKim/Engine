#include "pch.h"
#include "Frustum.h"

namespace JE {
BoundCheckResult Frustum::CheckBound(const Vector3& point) const
{
    // 절두체를 구성하는 6개의 평면에 대해 테스트 진행
    for (const Plane& p : Planes) {
        // 한 평면이라도 점이 바깥에 있으면 점은 외부에 있음을 판별하고 종료
        if (p.IsOutside(point))
            return BoundCheckResult::Outside;
        // 오차 범위 내에 거리가 0에 근접하면, 겹치는 것으로 판별하고 종료 (plane와 평면의 거리가 0에 가까운지 확인)
        else if (MathUtils::EqualsInTolerance(p.Distance(point), 0.f)) 
            return BoundCheckResult::Intersect;
    }

    // 모든 평면에 대해 테스트를 통과하면 안쪽으로 판별
    return BoundCheckResult::Inside;
}

BoundCheckResult Frustum::CheckBound(const Sphere& sphere) const
{
    // 절두체를 구성하는 6개의 평면에 대해 테스트 진행
    for (const Plane& p : Planes) {
        // 구가 평면의 바깥에 있을 때 구와 평면의 거리가 구의 반지름보다 크다면 완전히 바깥에 있다고 판단
        if (p.Distance(sphere.Center) > sphere.Radius) // radius는 양수이므로 Distance가 음수인 경우 무시되고, distance가 0보다 작다면 평면의 바깥이 아니라 상관없음.
            return BoundCheckResult::Outside;
        // 구와 평면의 거리가 구의 반지름보다 작거나 같으면 평면에 겹쳐있다고 판단
        else if (std::abs(p.Distance(sphere.Center)) <= sphere.Radius)
            return BoundCheckResult::Intersect;
    }

    // 모든 평면에 대해 테스트를 통과하면 안쪽으로 판단
    return BoundCheckResult::Inside;
}

BoundCheckResult Frustum::CheckBound(const Box& box) const
{
    // 절두체를 구성하는 6개의 평면에 대해 테스트 진행
    for (const Plane& p : Planes) {
        /*
            평면의 법선 벡터(Normal Vector)의 모든 요소가 양수(+, +, +)인 상황에서 AABB 영역이 바깥에 있다면, 평면에 가장 가까운 점은 최솟값이 된다.
            이때 최솟값과 평면의 D(거리)값은 양수가 나온다.

            법선 벡터의 모든 요소가 음수(-, -, -)인 상황에서 AABB 영역이 바깥에 있다면, 평면에서 가장 가까운 점은 최대값이 된다.
            이때 최댓값과 평면의 D(거리)값 또한 양수가 나온다.

            AABB 영역과 법선 벡터의 X, Y, Z축은 서로 직교하므로 각 축의 데이터는 독립적으로 동작한다.
            따라서 각 법선 벡터의 요소마다 양수(+)인 경우 최솟값을, 음수(-)인 경우 최대값을 사용하면 평면과 가장 가까운 AABB의 점을 구할 수 있다

            ex) 법선 벡터 = (+, -, +)라면 평면과 가장 가까운 AABB의 점 = (최솟값.X, 최댓값.Y, 최솟값.Z)가 된다.
        */

        // 박스(AABB) 영역에서 테스트할 두 점
        // 평면과 가장 가까운 AABB 영역의 점 (현재 Normal Vector가 (+, +, +)인 경우로 초기화
        Vector3 near = box.Min;
        // 평면과 가장 가까운 AABB 영역의 점의 정반대에 위치한 점 (반대 대각선 위치)
        Vector3 opposite = box.Max;

        // 평면의 Normal Vector 축의 부호를 파악해 두 점의 값을 변경
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

        // 평면과 AABB의 가장 가까운 점의 거리 테스트 값이 양수라면 평면의 외부
        if (p.Distance(near) > 0.f)
            return BoundCheckResult::Outside;
        // 두 점의 사이에 평면이 있다면 박스와 평면이 겹치는 것 (평면과 AABB의 가장 가까운 점의 거리가 음수가 나오고(평면의 내부와 평면의 위), 정반대 위치한 점과의 거리가 양수가 나오면(평면의 외부))
        if (p.Distance(near) <= 0.f && p.Distance(opposite) >= 0.f) 
            return BoundCheckResult::Intersect;
    }

    // 모든 평면에 대해 테스트를 통과하면 안쪽으로 판단
    return BoundCheckResult::Inside;
}
}