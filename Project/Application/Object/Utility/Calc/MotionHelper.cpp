#include "MotionHelper.h"
#include "../../../Engine/Math/Ease.h"

Vector3 MotionHelper::CurveBezier(const Vector3& start, const Vector3& end, float t, float height)
{
    // 中点
    Vector3 midPoint = (Vector3(start) + Vector3(end)) * 0.5f;
    midPoint.y += height;

    Vector3 b0b1 = Ease::Easing(Ease::EaseName::EaseInQuad,start,midPoint,t);
    Vector3 b1b2 = Ease::Easing(Ease::EaseName::EaseInQuad, midPoint, end, t);
    Vector3 result = Ease::Easing(Ease::EaseName::EaseInQuad, b0b1, b1b2, t);

    return Vector3(result);
}
