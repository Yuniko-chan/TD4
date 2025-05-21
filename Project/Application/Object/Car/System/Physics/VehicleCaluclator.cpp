#include "VehicleCaluclator.h"
#include "../../../Engine/Math/Matrix/Matrix4x4.h"

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
