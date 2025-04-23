
struct Segment
{
    // 原点
    float32_t3 origin_;
	// 終点までの差分ベクトル
    float32_t3 diff_;
    
};

struct ObjectData
{
    
    float32_t3 center_;
    float32_t3 otientatuons[3];
    float32_t3 size;
    
};

struct PolygonData
{
    
    float32_t3 positions[3];
	float32_t3 normal;
	uint32_t coursePolygonType;
    
};

struct OutputData
{
    
    float32_t3 extrusion;
	uint32_t drivingLocation;

};

ConstantBuffer<ObjectData> gObjectData : register(b0);

StructuredBuffer<PolygonData> gPolygonDatas : register(t0);

RWStructuredBuffer<OutputData> gOutputDatas : register(u0);

// 
bool CollisionCheck(PolygonData polygonData, Segment segment)
{
    
    Vector3 v01 = Subtract(triangle.
    vertices[1], triangle.
    vertices[0]);
    Vector3 v12 = Subtract(triangle.
    vertices[2], triangle.
    vertices[1]);
    Vector3 v20 = Subtract(triangle.
    vertices[0], triangle.
    vertices[2]);

	//平面を求める
    Plane plane = { Normalize(Cross(v01, v12)), 0.0f };
    plane.distance = plane.normal.x * v20.x + plane.normal.y * v20.y + plane.normal.z * v20.z;

	//平面との衝突確認
	//垂直判定のため、法線と線の内積を求める
    float dot = Dot(plane.normal, line.diff);

	//衝突していない
    if (dot == 0.0f)
    {
        return false;
    }

	//tを求める
    float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

    if (t < 0 || t > 1.0f)
    {
        return false;
    }

    Vector3 v1p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.
    vertices[1]);
    Vector3 v2p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.
    vertices[2]);
    Vector3 v0p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.
    vertices[0]);

    Vector3 cross01 = Cross(v01, v1p);
    Vector3 cross12 = Cross(v12, v2p);
    Vector3 cross20 = Cross(v20, v0p);

	//三角形との衝突確認
    if (Dot(cross01, plane.normal) >= 0.0f &&
		Dot(cross12, plane.normal) >= 0.0f &&
		Dot(cross20, plane.normal) >= 0.0f)
    {
        return true;
    }

    return false;
    
}

// 衝突確認 OBBの線分と三角ポリゴン
bool CollisionConfirmation(uint32_t index)
{
   
    // 
    bool result = false;
    
    // オブジェクトデータ
    ObjectData object = gObjectData;
    
    // 頂点求める
    float32_t3 vertices[8];
    
    // 左 上 前
    vertices[0] = (
        (-object.size.x * object.otientatuons[0].x + object.size.y * object.otientatuons[1].x +
					 -object.size.z * object.otientatuons[2].x),
		(-object.size.x * object.otientatuons[0].y + object.size.y * object.otientatuons[1].y +
					 -object.size.z * object.otientatuons[2].y),
		(-object.size.x * object.otientatuons[0].z + object.size.y * object.otientatuons[1].z +
					 -object.size.z * object.otientatuons[2].z)
    );

    // 左 上 後
    vertices[1] = (
        (-object.size.x * object.otientatuons[0].x + object.size.y * object.otientatuons[1].x +
					 object.size.z * object.otientatuons[2].x),
		(-object.size.x * object.otientatuons[0].y + object.size.y * object.otientatuons[1].y +
					 object.size.z * object.otientatuons[2].y),
		(-object.size.x * object.otientatuons[0].z + object.size.y * object.otientatuons[1].z +
					 object.size.z * object.otientatuons[2].z));

    // 右 上 前
    vertices[2] = (
        (object.size.x * object.otientatuons[0].x + object.size.y * object.otientatuons[1].x +
					 -object.size.z * object.otientatuons[2].x),
		(object.size.x * object.otientatuons[0].y + object.size.y * object.otientatuons[1].y +
					 -object.size.z * object.otientatuons[2].y),
		(object.size.x * object.otientatuons[0].z + object.size.y * object.otientatuons[1].z +
					 -object.size.z * object.otientatuons[2].z));

    // 右 上 後
    vertices[3] = (
        (object.size.x * object.otientatuons[0].x + object.size.y * object.otientatuons[1].x +
					 object.size.z * object.otientatuons[2].x),
		(object.size.x * object.otientatuons[0].y + object.size.y * object.otientatuons[1].y +
					 object.size.z * object.otientatuons[2].y),
		(object.size.x * object.otientatuons[0].z + object.size.y * object.otientatuons[1].z +
					 object.size.z * object.otientatuons[2].z));

    // 左 下 前
    vertices[4] = (
        (-object.size.x * object.otientatuons[0].x + -object.size.y * object.otientatuons[1].x +
					 -object.size.z * object.otientatuons[2].x),
		(-object.size.x * object.otientatuons[0].y + -object.size.y * object.otientatuons[1].y +
					 -object.size.z * object.otientatuons[2].y),
		(-object.size.x * object.otientatuons[0].z + -object.size.y * object.otientatuons[1].z +
					 -object.size.z * object.otientatuons[2].z));

    // 左 下 後
    vertices[5] = (
        (-object.size.x * object.otientatuons[0].x + -object.size.y * object.otientatuons[1].x +
					 object.size.z * object.otientatuons[2].x),
		(-object.size.x * object.otientatuons[0].y + -object.size.y * object.otientatuons[1].y +
					 object.size.z * object.otientatuons[2].y),
		(-object.size.x * object.otientatuons[0].z + -object.size.y * object.otientatuons[1].z +
					 object.size.z * object.otientatuons[2].z));

    // 右 下 前
    vertices[6] = (
        (object.size.x * object.otientatuons[0].x + -object.size.y * object.otientatuons[1].x +
					 -object.size.z * object.otientatuons[2].x),
		(object.size.x * object.otientatuons[0].y + -object.size.y * object.otientatuons[1].y +
					 -object.size.z * object.otientatuons[2].y),
		(object.size.x * object.otientatuons[0].z + -object.size.y * object.otientatuons[1].z +
					 -object.size.z * object.otientatuons[2].z)
                            );

    // 右 下 後
    vertices[7] = (
        (object.size.x * object.otientatuons[0].x + -object.size.y * object.otientatuons[1].x +
					 object.size.z * object.otientatuons[2].x),
		(object.size.x * object.otientatuons[0].y + -object.size.y * object.otientatuons[1].y +
					 object.size.z * object.otientatuons[2].y),
		(object.size.x * object.otientatuons[0].z + -object.size.y * object.otientatuons[1].z +
					 object.size.z * object.otientatuons[2].z)
                            );
    
    // 線分求める
    Segment segments[12];
    segments[0].origin_ = vertices[0];
    segments[0].diff_ = vertices[1] - vertices[0];
    
    segments[1].origin_ = vertices[1];
    segments[1].diff_ = vertices[3] - vertices[1];
    
    segments[2].origin_ = vertices[3];
    segments[2].diff_ = vertices[2] - vertices[3];
    
    segments[3].origin_ = vertices[2];
    segments[3].diff_ = vertices[0] - vertices[2];
    
    segments[4].origin_ = vertices[4];
    segments[4].diff_ = vertices[5] - vertices[4];
    
    segments[5].origin_ = vertices[5];
    segments[5].diff_ = vertices[7] - vertices[5];
    
    segments[6].origin_ = vertices[7];
    segments[6].diff_ = vertices[6] - vertices[7];
    
    segments[7].origin_ = vertices[6];
    segments[7].diff_ = vertices[4] - vertices[6];
    
    segments[8].origin_ = vertices[0];
    segments[8].diff_ = vertices[4] - vertices[0];
    
    segments[9].origin_ = vertices[1];
    segments[9].diff_ = vertices[5] - vertices[1];
    
    segments[10].origin_ = vertices[2];
    segments[10].diff_ = vertices[6] - vertices[2];
    
    segments[11].origin_ = vertices[3];
    segments[11].diff_ = vertices[7] - vertices[3];
    
    PolygonData polygonData = gPolygonDatas[index];
    
    // 線分と三角形の衝突確認
    for (uint32_t i = 0; i < 12; ++i)
    {
       result = CollisionCheck(polygonData, segments[i]);
        if (result)
        {
            break;
        }

    }
    
    
    return result;
    
}


[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{
    
    uint32_t index = dispatchId.x;
    
    // 衝突確認 OBBの線分と三角ポリゴン
    
    
    // 衝突確認 OBBの中に三角の頂点があるか
    
    // 押し出し処理
    
    
    // 押し出し値追加
    gOutputDatas[index].extrusion;
    gOutputDatas[index].drivingLocation = gPolygonDatas[index].coursePolygonType;

}