#include "Quaternion.h"
#include <cmath>

Quaternion Quaternion::Add(const Quaternion& q0, const Quaternion& q1)
{

	Quaternion result = {};

	result.x = q0.x + q1.x;
	result.y = q0.y + q1.y;
	result.z = q0.z + q1.z;
	result.w = q0.w + q1.w;

	return result;
}

Quaternion Quaternion::Subtract(const Quaternion& q0, const Quaternion& q1)
{

	Quaternion result = {};

	result.x = q0.x - q1.x;
	result.y = q0.y - q1.y;
	result.z = q0.z - q1.z;
	result.w = q0.w - q1.w;

	return result;

}

Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs)
{

	Quaternion result = {};

	Vector3 q = { lhs.x, lhs.y, lhs.z, };
	Vector3 r = { rhs.x, rhs.y, rhs.z, };
	Vector3 resultVector = Vector3::Add(Vector3::Cross(q, r), Vector3::Add(Vector3::Multiply(rhs.w, q), Vector3::Multiply(lhs.w, r)));

	result.x = resultVector.x;
	result.y = resultVector.y;
	result.z = resultVector.z;
	result.w = lhs.w * rhs.w - Vector3::Dot(q, r);

	return result;
}

Quaternion Quaternion::Multiply(const Quaternion& q, float s)
{

	Quaternion result = {};
	result.x = q.x * s;
	result.y = q.y * s;
	result.z = q.z * s;
	result.w = q.w * s;

	return result;
}

Quaternion Quaternion::Multiply(float s, const Quaternion& q)
{
	return Multiply(q, s);
}

Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion result = { 0.0f,0.0f,0.0f,1.0f };
	return result;
}

Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
{
	Quaternion result = { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
	return result;
}

float Quaternion::Norm(const Quaternion& quaternion)
{
	float result = sqrtf(quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w);

	return result;
}

Quaternion Quaternion::Normalize(const Quaternion& quaternion)
{
	Quaternion result = {};
	float norm = Norm(quaternion);

	if (quaternion.x != 0.0) {
		result.x = quaternion.x / norm;
	}
	else {
		result.x = 0.0f;
	}

	if (quaternion.y != 0.0) {
		result.y = quaternion.y / norm;
	}
	else {
		result.y = 0.0f;
	}

	if (quaternion.z != 0.0) {
		result.z = quaternion.z / norm;
	}
	else {
		result.z = 0.0f;
	}

	if (quaternion.w != 0.0) {
		result.w = quaternion.w / norm;
	}
	else {
		result.w = 0.0f;
	}

	return result;

}

Quaternion Quaternion::Inverse(const Quaternion& quaternion)
{

	Quaternion result = {};
	Quaternion conjugate = Conjugate(quaternion);

	float norm = Norm(quaternion);

	if (norm != 0.0f) {
		result.x = conjugate.x / (norm * norm);
		result.y = conjugate.y / (norm * norm);
		result.z = conjugate.z / (norm * norm);
		result.w = conjugate.w / (norm * norm);
	}

	return result;
}

Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{

	Quaternion result = { };
	Vector3 vector = Vector3::Multiply(std::sinf(angle / 2.0f), axis);
	result = { vector.x, vector.y, vector.z, std::cosf(angle / 2.0f) };

	return result;

}

Vector3 Quaternion::RotateVector(const Vector3& vector, const Quaternion& quaternion)
{

	Vector3 result = {};

	Quaternion r = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion conjugate = Conjugate(quaternion);
	Quaternion newQuaternion = Multiply(Multiply(quaternion, r), conjugate);

	result.x = newQuaternion.x;
	result.y = newQuaternion.y;
	result.z = newQuaternion.z;

	return result;
}

Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion)
{

	Matrix4x4 result = {};
	float x = quaternion.x;
	float y = quaternion.y;
	float z = quaternion.z;
	float w = quaternion.w;

	result.m[0][0] = std::powf(w, 2.0f) + std::powf(x, 2.0f) - std::powf(y, 2.0f) - std::powf(z, 2.0f);
	result.m[0][1] = 2 * (x * y + w * z);
	result.m[0][2] = 2 * (x * z - w * y);
	result.m[0][3] = 0.0f;
	result.m[1][0] = 2 * (x * y - w * z);
	result.m[1][1] = std::powf(w, 2.0f) - std::powf(x, 2.0f) + std::powf(y, 2.0f) - std::powf(z, 2.0f);
	result.m[1][2] = 2 * (y * z + w * x);
	result.m[1][3] = 0.0f;
	result.m[2][0] = 2 * (x * z + w * y);
	result.m[2][1] = 2 * (y * z - w * x);
	result.m[2][2] = std::powf(w, 2.0f) - std::powf(x, 2.0f) - std::powf(y, 2.0f) + std::powf(z, 2.0f);
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;

}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{

	const float kEpsilon = 0.0005f;
	float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w; // q0とq1の内積

	Quaternion use_q0 = q0;

	if (dot < 0.0f) {
		use_q0 = Multiply(q0, -1.0f);
		dot = -dot;
	}

	if (dot >= 1.0f - kEpsilon) {
		Quaternion result = Add(Multiply(use_q0, 1.0f - t), Multiply(q1, t));
		return result;
	}

	// なす角を求める
	float theta = std::acosf(dot);

	// thetaとsinを使って補間係数scale0,scale1を求める
	float scale0 = std::sinf((1.0f - t) * theta) / std::sinf(theta);
	float scale1 = std::sinf(t * theta) / std::sinf(theta);

	Quaternion result = Add(Multiply(use_q0, scale0), Multiply(q1, scale1));

	return result;

}

Quaternion Quaternion::DirectionToDirection(const Vector3& v0, const Vector3& v1)
{

	// n
	Vector3 n = Vector3::Cross(v0, v1);

	// cosTheta
	float cosTheta = Vector3::Dot(v0, v1);

	// なす角を求める
	float theta = acosf(cosTheta);

	Quaternion result = {
		n.x * sinf(theta / 2.0f),
		n.y * sinf(theta / 2.0f),
		n.z * sinf(theta / 2.0f),
		cosf(theta)
	};

	return result;

}

Quaternion Quaternion::MatrixToQuaternion(const Matrix4x4& mat)
{
	Quaternion q;
	//Quaternion p = {};
	//p.x = mat.m[0][0] - mat.m[1][1] - mat.m[2][2] + 1;
	//p.y = -mat.m[0][0] + mat.m[1][1] - mat.m[2][2] + 1;
	//p.z = -mat.m[0][0] - mat.m[1][1] + mat.m[2][2] + 1;
	//p.w = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1;

	//int selected = 0;
	//float max = p.x;
	//if (max < p.y) {
	//	selected = 1;
	//	max = p.y;
	//}
	//if (max < p.z) {
	//	selected = 2;
	//	max = p.z;
	//}
	//if (max < p.w) {
	//	selected = 3;
	//	max = p.w;
	//}
	//float d = 0.0f;
	//// X
	//if (selected == 0) {
	//	const float EPSILON = 1e-6f;
	//	d = (max > EPSILON) ? 1.0f / (4.0f * max) : 0.0f;
	//	//d = 1 / (4 * p.x);

	//	q.x = std::sqrtf(p.x) * 0.5f;
	//	q.y = (mat.m[1][0] + mat.m[0][1]) * d;
	//	q.z = (mat.m[0][2] + mat.m[2][0]) * d;
	//	q.w = (mat.m[2][1] - mat.m[1][2]) * d;
	//}
	//// Y
	//else if (selected == 1) {
	//	const float EPSILON = 1e-6f;
	//	d = (max > EPSILON) ? 1.0f / (4.0f * max) : 0.0f;

	//	q.x = (mat.m[1][0] + mat.m[0][1]) * d;
	//	q.y = std::sqrtf(p.y) * 0.5f;
	//	q.z = (mat.m[2][1] + mat.m[1][2]) * d;
	//	q.w = (mat.m[0][2] - mat.m[2][0]) * d;
	//}
	//// Z
	//else if (selected == 2) {
	//	const float EPSILON = 1e-6f;
	//	d = (max > EPSILON) ? 1.0f / (4.0f * max) : 0.0f;

	//	q.x = (mat.m[0][2] + mat.m[2][0]) * d;
	//	q.y = (mat.m[2][1] + mat.m[1][2]) * d;
	//	q.z = std::sqrtf(p.z) * 0.5f;
	//	q.w = (mat.m[1][0] - mat.m[0][1]) * d;
	//}
	//// W
	//else if (selected == 3) {
	//	const float EPSILON = 1e-6f;
	//	d = (max > EPSILON) ? 1.0f / (4.0f * max) : 0.0f;

	//	q.x = (mat.m[2][1] - mat.m[1][2]) * d;
	//	q.y = (mat.m[0][2] - mat.m[2][0]) * d;
	//	q.z = (mat.m[1][0] - mat.m[0][1]) * d;
	//	q.w = std::sqrtf(p.w) * 0.5f;
	//}

	//return Quaternion::Normalize(q);
	// トレースの値
	float trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];

	// W
	if (trace > 0.0f) {
		float fRoot = sqrtf(trace + 1.0f); // s = 2qw

		q.w = 0.5f * fRoot;

		fRoot = 0.5f / fRoot;	// 1 / (4 * qw)
		q.x = (mat.m[2][1] - mat.m[1][2]) * fRoot;
		q.y = (mat.m[0][2] - mat.m[2][0]) * fRoot;
		q.z = (mat.m[1][0] - mat.m[0][1]) * fRoot;
	}
	// X
	else if ((mat.m[0][0] > mat.m[1][1]) && (mat.m[0][0] > mat.m[2][2])) {
		float fRoot = sqrtf(1.0f + mat.m[0][0] - mat.m[1][1] - mat.m[2][2]); // s=2qx

		q.x = 0.5f * fRoot;

		fRoot = 0.5f / fRoot;

		q.w = (mat.m[2][1] - mat.m[1][2]) * fRoot;
		q.y = (mat.m[0][1] + mat.m[1][0]) * fRoot;
		q.z = (mat.m[0][2] + mat.m[2][0]) * fRoot;
	}
	// Y
	else if (mat.m[1][1] > mat.m[2][2]) {
		float fRoot = sqrtf(1.0f + mat.m[1][1] - mat.m[0][0] - mat.m[2][2]); // s=2qy

		q.y = 0.5f * fRoot;

		fRoot = 0.5f / fRoot;

		q.w = (mat.m[0][2] - mat.m[2][0]) * fRoot;
		q.x = (mat.m[0][1] + mat.m[1][0]) * fRoot;
		q.z = (mat.m[1][2] + mat.m[2][1]) * fRoot;
	}
	// Z
	else {
		float fRoot = sqrtf(1.0f + mat.m[2][2] - mat.m[0][0] - mat.m[1][1]); // s=4*qz

		q.z = 0.5f * fRoot;

		fRoot = 0.5f / fRoot;

		q.w = (mat.m[1][0] - mat.m[0][1]) * fRoot;
		q.x = (mat.m[0][2] + mat.m[2][0]) * fRoot;
		q.y = (mat.m[1][2] + mat.m[2][1]) * fRoot;
	}

	return Quaternion(q);
}

Quaternion Quaternion::LookRotation(const Vector3& direction)
{
	// 前方
	Vector3 forward = Vector3::Normalize(direction);
	// ワールドの上
	Vector3 worldUp = { 0.0f, 1.0f, 0.0f };
	// 右ベクトル
	Vector3 right = Vector3::Normalize(Vector3::Cross(worldUp, forward));
	// 前方とローカルの上が平行な場合右が0になるため対応する
	if (Vector3::Length(right) < 0.0001f) {
		worldUp = { 0.0f, 0.0f, -1.0f };
		right = Vector3::Normalize(Vector3::Cross(worldUp, forward));
	}
	// 前方と右のベクトルからローカルの上軸ベクトル計算まで
	Vector3 localUp = Vector3::Normalize(Vector3::Cross(forward, right));

	// 各要素から行列を構築
	Matrix4x4 rotationMatrix = {};
	// 右
	rotationMatrix.m[0][0] = right.x;
	rotationMatrix.m[1][0] = right.y;
	rotationMatrix.m[2][0] = right.z;
	rotationMatrix.m[3][0] = 0.0f;
	// 上
	rotationMatrix.m[0][1] = localUp.x;
	rotationMatrix.m[1][1] = localUp.y;
	rotationMatrix.m[2][1] = localUp.z;
	rotationMatrix.m[3][1] = 0.0f;
	// 前
	rotationMatrix.m[0][2] = forward.x;
	rotationMatrix.m[1][2] = forward.y;
	rotationMatrix.m[2][2] = forward.z;
	rotationMatrix.m[3][2] = 0.0f;

	// 行列から回転クォータニオンの構築
	return MatrixToQuaternion(rotationMatrix);
}

Quaternion Quaternion::operator+(const Quaternion& v)
{

	Quaternion result = *this;

	result = Add(result, v);

	return result;

}

void Quaternion::operator+=(const Quaternion& v)
{

	Quaternion result = *this;

	result = Add(result, v);

	*this = result;

}

Quaternion Quaternion::operator-(const Quaternion& v)
{
	
	Quaternion result = *this;

	result = Subtract(result, v);

	return result;

}

void Quaternion::operator-=(const Quaternion& v)
{

	Quaternion result = *this;

	result = Subtract(result, v);

	*this = result;

}

Quaternion Quaternion::operator*(float v)
{

	Quaternion result = *this;

	result = Multiply(result, v);

	return result;

}

void Quaternion::operator*=(float v)
{

	Quaternion result = *this;

	result = Multiply(result, v);

	*this = result;

}

Quaternion Quaternion::operator*(const Quaternion& v)
{

	Quaternion result = *this;

	result = Multiply(result, v);

	return result;

}

void Quaternion::operator*=(const Quaternion& v)
{

	Quaternion result = *this;

	result = Multiply(result, v);

	*this = result;

}
