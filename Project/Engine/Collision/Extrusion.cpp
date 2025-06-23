#include "Extrusion.h"

Vector3 Extrusion::OBBAndOBB(OBB* pushedOut, OBB* pushOut)
{

    // pushOutを分解
        
    // OBB平面作成
    Vector3 planeYZ = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushOut->otientatuons_[0].x), 1.0f - std::fabs(pushOut->otientatuons_[0].y), 1.0f - std::fabs(pushOut->otientatuons_[0].z) });
    Vector3 planeXZ = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushOut->otientatuons_[1].x), 1.0f - std::fabs(pushOut->otientatuons_[1].y), 1.0f - std::fabs(pushOut->otientatuons_[1].z) });
    Vector3 planeXY = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushOut->otientatuons_[2].x), 1.0f - std::fabs(pushOut->otientatuons_[2].y), 1.0f - std::fabs(pushOut->otientatuons_[2].z) });

    Vector3 size = pushOut->size_;
    Vector3 center = pushOut->center_;

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

    Vector3 planeVertices[6][4];

    // 前
    planeVertices[0][0] = vertices[4];
    planeVertices[0][1] = vertices[0];
    planeVertices[0][2] = vertices[6];
    planeVertices[0][3] = vertices[2];

    // 後
    planeVertices[1][0] = vertices[7];
    planeVertices[1][1] = vertices[3];
    planeVertices[1][2] = vertices[5];
    planeVertices[1][3] = vertices[1];

    // 右
    planeVertices[2][0] = vertices[6];
    planeVertices[2][1] = vertices[2];
    planeVertices[2][2] = vertices[7];
    planeVertices[2][3] = vertices[3];

    // 左
    planeVertices[3][0] = vertices[5];
    planeVertices[3][1] = vertices[1];
    planeVertices[3][2] = vertices[4];
    planeVertices[3][3] = vertices[0];

    // 上
    planeVertices[4][0] = vertices[0];
    planeVertices[4][1] = vertices[1];
    planeVertices[4][2] = vertices[3];
    planeVertices[4][3] = vertices[2];

    // 下
    planeVertices[5][0] = vertices[5];
    planeVertices[5][1] = vertices[4];
    planeVertices[5][2] = vertices[6];
    planeVertices[5][3] = vertices[7];

    Vector3 extrusion = { 0.0f,0.0f,0.0f };

    for (uint32_t i = 0; i < 6; ++i) {

        Plane plane;

        Vector3 v01 = planeVertices[i][1] - planeVertices[i][0];
        Vector3 v12 = planeVertices[i][2] - planeVertices[i][1];
        Vector3 v20 = planeVertices[i][0] - planeVertices[i][2];

        //平面を求める
        plane.normal_ = Vector3::Normalize(Vector3::Cross(v01, v12));
        plane.distance_ = Vector3::Dot(planeVertices[i][0], plane.normal_);

        extrusion += OBBAndPlane(pushedOut, &plane, planeVertices[i]);

    }

    return extrusion;
	
}

Vector3 Extrusion::OBBAndPlane(OBB* pushedOut, Plane* pushOut, Vector3 planeVertices[4])
{

    // OBB平面作成
    //Vector3 planeYZ = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushedOut->otientatuons_[0].x), 1.0f - std::fabs(pushedOut->otientatuons_[0].y), 1.0f - std::fabs(pushedOut->otientatuons_[0].z) });
    //Vector3 planeXZ = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushedOut->otientatuons_[1].x), 1.0f - std::fabs(pushedOut->otientatuons_[1].y), 1.0f - std::fabs(pushedOut->otientatuons_[1].z) });
    //Vector3 planeXY = Vector3::Normalize(Vector3{ 1.0f - std::fabs(pushedOut->otientatuons_[2].x), 1.0f - std::fabs(pushedOut->otientatuons_[2].y), 1.0f - std::fabs(pushedOut->otientatuons_[2].z) });

    // 軸固定
    Vector3 planeYZ = Vector3{ 0.0f, 0.5f,0.5f };
    Vector3 planeXZ = Vector3{ 0.5f, 0.0f,0.5f };
    Vector3 planeXY = Vector3{ 0.5f, 0.5f,0.0f };

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
