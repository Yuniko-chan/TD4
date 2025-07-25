#include "Extrusion.h"

Vector3 Extrusion::OBBAndOBB(OBB* pushedOut, OBB* pushOut)
{
	Vector3* pushOutOtientatuons = pushOut->otientatuons_;

	Vector3 pushOutSize = pushOut->size_;

	Vector3 pushOutCenter = pushOut->center_;

	//頂点
	Vector3 vertex[8];

	vertex[0] = {
		(-pushOutSize.x * pushOutOtientatuons[0].x + pushOutSize.y * pushOutOtientatuons[1].x +
					 -pushOutSize.z * pushOutOtientatuons[2].x),
		(-pushOutSize.x * pushOutOtientatuons[0].y + pushOutSize.y * pushOutOtientatuons[1].y +
					 -pushOutSize.z * pushOutOtientatuons[2].y),
		(-pushOutSize.x * pushOutOtientatuons[0].z + pushOutSize.y * pushOutOtientatuons[1].z +
					 -pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[1] = {
	   (-pushOutSize.x * pushOutOtientatuons[0].x + pushOutSize.y * pushOutOtientatuons[1].x +
					 pushOutSize.z * pushOutOtientatuons[2].x),
		(-pushOutSize.x * pushOutOtientatuons[0].y + pushOutSize.y * pushOutOtientatuons[1].y +
					 pushOutSize.z * pushOutOtientatuons[2].y),
		(-pushOutSize.x * pushOutOtientatuons[0].z + pushOutSize.y * pushOutOtientatuons[1].z +
					 pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[2] = {
		(pushOutSize.x * pushOutOtientatuons[0].x + pushOutSize.y * pushOutOtientatuons[1].x +
					 -pushOutSize.z * pushOutOtientatuons[2].x),
		(pushOutSize.x * pushOutOtientatuons[0].y + pushOutSize.y * pushOutOtientatuons[1].y +
					 -pushOutSize.z * pushOutOtientatuons[2].y),
		(pushOutSize.x * pushOutOtientatuons[0].z + pushOutSize.y * pushOutOtientatuons[1].z +
					 -pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[3] = {
		(pushOutSize.x * pushOutOtientatuons[0].x + pushOutSize.y * pushOutOtientatuons[1].x +
					 pushOutSize.z * pushOutOtientatuons[2].x),
		(pushOutSize.x * pushOutOtientatuons[0].y + pushOutSize.y * pushOutOtientatuons[1].y +
					 pushOutSize.z * pushOutOtientatuons[2].y),
		(pushOutSize.x * pushOutOtientatuons[0].z + pushOutSize.y * pushOutOtientatuons[1].z +
					 pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[4] = {
		(-pushOutSize.x * pushOutOtientatuons[0].x + -pushOutSize.y * pushOutOtientatuons[1].x +
					 -pushOutSize.z * pushOutOtientatuons[2].x),
		(-pushOutSize.x * pushOutOtientatuons[0].y + -pushOutSize.y * pushOutOtientatuons[1].y +
					 -pushOutSize.z * pushOutOtientatuons[2].y),
		(-pushOutSize.x * pushOutOtientatuons[0].z + -pushOutSize.y * pushOutOtientatuons[1].z +
					 -pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[5] = {
		(-pushOutSize.x * pushOutOtientatuons[0].x + -pushOutSize.y * pushOutOtientatuons[1].x +
					 pushOutSize.z * pushOutOtientatuons[2].x),
		(-pushOutSize.x * pushOutOtientatuons[0].y + -pushOutSize.y * pushOutOtientatuons[1].y +
					 pushOutSize.z * pushOutOtientatuons[2].y),
		(-pushOutSize.x * pushOutOtientatuons[0].z + -pushOutSize.y * pushOutOtientatuons[1].z +
					 pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[6] = {
		(pushOutSize.x * pushOutOtientatuons[0].x + -pushOutSize.y * pushOutOtientatuons[1].x +
					 -pushOutSize.z * pushOutOtientatuons[2].x),
		(pushOutSize.x * pushOutOtientatuons[0].y + -pushOutSize.y * pushOutOtientatuons[1].y +
					 -pushOutSize.z * pushOutOtientatuons[2].y),
		(pushOutSize.x * pushOutOtientatuons[0].z + -pushOutSize.y * pushOutOtientatuons[1].z +
					 -pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[7] = {
		(pushOutSize.x * pushOutOtientatuons[0].x + -pushOutSize.y * pushOutOtientatuons[1].x +
					 pushOutSize.z * pushOutOtientatuons[2].x),
		(pushOutSize.x * pushOutOtientatuons[0].y + -pushOutSize.y * pushOutOtientatuons[1].y +
					 pushOutSize.z * pushOutOtientatuons[2].y),
		(pushOutSize.x * pushOutOtientatuons[0].z + -pushOutSize.y * pushOutOtientatuons[1].z +
					 pushOutSize.z * pushOutOtientatuons[2].z) };

	Vector3 useVertex[6][4]{
		{vertex[2], vertex[3], vertex[7], vertex[6]},
		{vertex[1], vertex[0], vertex[4], vertex[5]},
		{vertex[0], vertex[1], vertex[3], vertex[2]},
		{vertex[6], vertex[7], vertex[5], vertex[4]},
		{vertex[5], vertex[7], vertex[3], vertex[1]},
		{vertex[0], vertex[2], vertex[6], vertex[4]},
	};

	// 氷から平面
	Vector3 icePlaneNormal[6] = {};
	icePlaneNormal[0] = pushOutOtientatuons[0];
	icePlaneNormal[1] = pushOutOtientatuons[0] * -1;
	icePlaneNormal[2] = pushOutOtientatuons[1];
	icePlaneNormal[3] = pushOutOtientatuons[1] * -1;
	icePlaneNormal[4] = pushOutOtientatuons[2];
	icePlaneNormal[5] = pushOutOtientatuons[2] * -1;
	float iceDistance[6] = {};
	iceDistance[0] = pushOutSize.x;
	iceDistance[1] = pushOutSize.x;
	iceDistance[2] = pushOutSize.y;
	iceDistance[3] = pushOutSize.y;
	iceDistance[4] = pushOutSize.z;
	iceDistance[5] = pushOutSize.z;

	//線分作成
	Vector3 origin = { 0.0f,0.0f,0.0f };
	Vector3 diff = pushedOut->center_ - pushOutCenter;
	size_t num = 0;

	for (size_t i = 0; i < 6; i++) {

		// 垂直判定のため、法線と線の内積を求める
		float dot = Vector3::Dot(icePlaneNormal[i], diff);

		// 衝突しているかも
		if (dot != 0.0f) {

			// tを求める
			float t = (iceDistance[i] - Vector3::Dot(origin, icePlaneNormal[i])) / dot;

			if (t >= 0) {

				//平面の中か
				Vector3 v01 = useVertex[i][1] - useVertex[i][0];
				Vector3 v12 = useVertex[i][2] - useVertex[i][1];
				Vector3 v23 = useVertex[i][3] - useVertex[i][2];
				Vector3 v30 = useVertex[i][0] - useVertex[i][3];

				Vector3 v1p = origin + (diff * t) - useVertex[i][1];
				Vector3 v2p = origin + (diff * t) - useVertex[i][2];
				Vector3 v3p = origin + (diff * t) - useVertex[i][3];
				Vector3 v0p = origin + (diff * t) - useVertex[i][0];

				Vector3 cross01 = Vector3::Cross(v01, v1p);
				Vector3 cross12 = Vector3::Cross(v12, v2p);
				Vector3 cross23 = Vector3::Cross(v23, v3p);
				Vector3 cross30 = Vector3::Cross(v30, v0p);

				if (Vector3::Dot(cross01, icePlaneNormal[i]) >= 0.0f &&
					Vector3::Dot(cross12, icePlaneNormal[i]) >= 0.0f &&
					Vector3::Dot(cross23, icePlaneNormal[i]) >= 0.0f &&
					Vector3::Dot(cross30, icePlaneNormal[i]) >= 0.0f) {

					num = i;
					break;
				}
			}

		}

	}

	//平面とObb
	float r = 0.0f;
	Vector3* playerOtientatuons = pushedOut->otientatuons_;
	float playersize[3] = {
		pushedOut->size_.x,
		pushedOut->size_.y,
		pushedOut->size_.z,
	};
	for (size_t i = 0; i < 3; i++) {
		r += std::fabsf(Vector3::Dot(playerOtientatuons[i] * playersize[i], icePlaneNormal[num]));
	}

	//平面とobbの距離
	Vector3 playerPos = pushedOut->center_;
	Vector3 planePos =
		pushOutCenter + (icePlaneNormal[num] * iceDistance[num]);

	float s = Vector3::Dot(playerPos - planePos, icePlaneNormal[num]);
	float distance = 0.0f;
	if (s > 0) {
		distance = r - std::fabsf(s);
	}
	else {
		distance = r + std::fabsf(s);
	}

	return icePlaneNormal[num] * distance;
	
}

Vector3 Extrusion::OBBAndPlane(OBB* pushedOut, Plane* pushOut, Vector3 planeVertices[4])
{

    // OBB平面作成
    Vector3 planeYZ = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushedOut->otientatuons_[0].x), 1.0f - std::fabs(pushedOut->otientatuons_[0].y), 1.0f - std::fabs(pushedOut->otientatuons_[0].z) });
    Vector3 planeXZ = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushedOut->otientatuons_[1].x), 1.0f - std::fabs(pushedOut->otientatuons_[1].y), 1.0f - std::fabs(pushedOut->otientatuons_[1].z) });
    Vector3 planeXY = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushedOut->otientatuons_[2].x), 1.0f - std::fabs(pushedOut->otientatuons_[2].y), 1.0f - std::fabs(pushedOut->otientatuons_[2].z) });

    // 軸固定
    //Vector3 planeYZ = pushedOut->otientatuons_[0];
    //Vector3 planeXZ = pushedOut->otientatuons_[1];
    //Vector3 planeXY = pushedOut->otientatuons_[2];

    Vector3 size = pushedOut->size_;
    Vector3 center = pushedOut->center_;

    Vector3 planeNormal = pushOut->normal_;
    float planeDistance = pushOut->distance_;

    float r = 0.0f;

    r += abs(Vector3::Dot(planeYZ * size.x, planeNormal));
    r += abs(Vector3::Dot(planeXZ * size.y, planeNormal));
    r += abs(Vector3::Dot(planeXY * size.z, planeNormal));

    //平面とobbの距離(怪しい)
    Vector3 planePos = planeNormal * planeDistance;

    float s = Vector3::Dot(center - planePos, planeNormal);
    float distance = 0.0f;
    if (s > 0.0f)
    {
        distance = r - abs(s);
    }
    else
    {
        distance = r + abs(s);
    }

    // 衝突確認
    Segment segment;
    segment.origin_ = center;
    segment.diff_ = planeNormal * distance;

    // 頂点求める
    Vector3 vertices[8];

    // 左 上 前
    vertices[0] = {
        (-size.x * planeYZ.x + size.y * planeXZ.x + -size.z * planeXY.x),
        (-size.x * planeYZ.y + size.y * planeXZ.y + -size.z * planeXY.y),
        (-size.x * planeYZ.z + size.y * planeXZ.z + -size.z * planeXY.z)
    };

    // 左 上 後
    vertices[1] = {
        (-size.x * planeYZ.x + size.y * planeXZ.x + size.z * planeXY.x),
        (-size.x * planeYZ.y + size.y * planeXZ.y + size.z * planeXY.y),
        (-size.x * planeYZ.z + size.y * planeXZ.z + size.z * planeXY.z)
    };

    // 右 上 前
    vertices[2] = {
        (size.x * planeYZ.x + size.y * planeXZ.x + -size.z * planeXY.x),
        (size.x * planeYZ.y + size.y * planeXZ.y + -size.z * planeXY.y),
        (size.x * planeYZ.z + size.y * planeXZ.z + -size.z * planeXY.z)
    };

    // 右 上 後
    vertices[3] = {
        (size.x * planeYZ.x + size.y * planeXZ.x + size.z * planeXY.x),
        (size.x * planeYZ.y + size.y * planeXZ.y + size.z * planeXY.y),
        (size.x * planeYZ.z + size.y * planeXZ.z + size.z * planeXY.z)
    };

    // 左 下 前
    vertices[4] = {
        (-size.x * planeYZ.x + -size.y * planeXZ.x + -size.z * planeXY.x),
        (-size.x * planeYZ.y + -size.y * planeXZ.y + -size.z * planeXY.y),
        (-size.x * planeYZ.z + -size.y * planeXZ.z + -size.z * planeXY.z)
    };

    // 左 下 後
    vertices[5] = {
        (-size.x * planeYZ.x + -size.y * planeXZ.x + size.z * planeXY.x),
        (-size.x * planeYZ.y + -size.y * planeXZ.y + size.z * planeXY.y),
        (-size.x * planeYZ.z + -size.y * planeXZ.z + size.z * planeXY.z)
    };

    // 右 下 前
    vertices[6] = {
        (size.x * planeYZ.x + -size.y * planeXZ.x + -size.z * planeXY.x),
        (size.x * planeYZ.y + -size.y * planeXZ.y + -size.z * planeXY.y),
        (size.x * planeYZ.z + -size.y * planeXZ.z + -size.z * planeXY.z)
    };

    // 右 下 後
    vertices[7] = {
        (size.x * planeYZ.x + -size.y * planeXZ.x + size.z * planeXY.x),
        (size.x * planeYZ.y + -size.y * planeXZ.y + size.z * planeXY.y),
        (size.x * planeYZ.z + -size.y * planeXZ.z + size.z * planeXY.z)
    };

    bool collisionCheck = false;

    Vector3 planeVertices0[3] = { planeVertices[0],planeVertices[1], planeVertices[2] }; // 左下、左上、右下
    Vector3 planeVertices1[3] = { planeVertices[1],planeVertices[3], planeVertices[2] }; // 左上、右上、右下

    for (uint32_t i = 0; i < 8; ++i)
    {
        segment.origin_ = center + vertices[i];
        collisionCheck = 
            OBBAndPlaneCollisionCheck(planeVertices0, segment, planeNormal, planeDistance) ||
            OBBAndPlaneCollisionCheck(planeVertices1, segment, planeNormal, planeDistance);

        if (collisionCheck)
        {
            break;
        }

    }

    if (abs(s) - r < 0.0f && collisionCheck)
    {
        return planeNormal * distance;
    }

    return Vector3{ 0.0f,0.0f,0.0f };

}

bool Extrusion::OBBAndPlaneCollisionCheck(Vector3 planeVertices[3], const Segment& segment, const Vector3& planeNormal, const float& planeDistance)
{

    float dotValue = Vector3::Dot(planeNormal, segment.diff_);

    //衝突していない
    if (dotValue == 0.0f)
    {
        return false;
    }
    else {

        Vector3 v01 = planeVertices[1] - planeVertices[0];
        Vector3 v12 = planeVertices[2] - planeVertices[1];
        Vector3 v20 = planeVertices[0] - planeVertices[2];

        //tを求める
        float t = (planeDistance - Vector3::Dot(segment.origin_, planeNormal)) / dotValue;

        Vector3 vp = Vector3::Add(segment.origin_,Vector3::Multiply(segment.diff_, t));

        Vector3 v1p = vp - planeVertices[1];
        Vector3 v2p = vp - planeVertices[2];
        Vector3 v0p = vp - planeVertices[0];

        Vector3 cross01 = Vector3::Cross(v01, v1p);
        Vector3 cross12 = Vector3::Cross(v12, v2p);
        Vector3 cross20 = Vector3::Cross(v20, v0p);

        //三角形との衝突確認
        if (Vector3::Dot(cross01, planeNormal) >= 0.0f &&
            Vector3::Dot(cross12, planeNormal) >= 0.0f &&
            Vector3::Dot(cross20, planeNormal) >= 0.0f)
        {
            return true;
        }

    }

    return false;

}
