#include "VehicleCaluclator.h"
#include "../../../Engine/Math/Matrix/Matrix4x4.h"
#include "../../../Engine/Math/DeltaTime.h"

Vector3 VehicleCaluclator::VelocityCalculator()
{
    return Vector3();
}

Vector3 VehicleCaluclator::SnapToZero(const Vector3& vector, const float epsilon)
{
    Vector3 result = vector;
    if (std::fabsf(result.x) < epsilon) result.x = 0.0f;
    if (std::fabsf(result.y) < epsilon) result.y = 0.0f;
    if (std::fabsf(result.z) < epsilon) result.z = 0.0f;
    return Vector3(result);
}

Vector3 VehicleCaluclator::RotateVector(const Vector3& velocity, const float euler)
{
    return Vector3(Matrix4x4::TransformNormal(velocity, Matrix4x4::MakeRotateYMatrix(euler)));
}

Vector3 VehicleCaluclator::ExponentialInterpolate(const Vector3& current, const Vector3& end, float decay)
{
    float factor = (1.0f - std::exp(-decay * kDeltaTime_));
    Vector3 v1 = current;
    Vector3 v2 = end;
    return v1 + (v2 - v1) * factor;
}
