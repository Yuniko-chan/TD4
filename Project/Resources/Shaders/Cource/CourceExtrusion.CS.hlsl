
struct Segment
{
    // 原点
    float32_t3 origin;
	// 終点までの差分ベクトル
    float32_t3 diff;
    
};

struct ObjectData
{
    uint32_t indexMax;
    float32_t3 center;
    float32_t3 size;
    float32_t3 otientatuonsX;
    float32_t3 otientatuonsY;
    float32_t3 otientatuonsZ;
    
};

struct PolygonData
{
    
    float32_t3 positions0;
    float32_t3 positions1;
    float32_t3 positions2;
	float32_t3 normal;
    float32_t2 texcoord;
    
};

struct OutputData
{
    
    float32_t3 extrusion;
	uint32_t drivingLocation;
    uint32_t collided;

};

ConstantBuffer<ObjectData> gObjectData : register(b0);

StructuredBuffer<PolygonData> gPolygonDatas : register(t0);

RWStructuredBuffer<OutputData> gOutputDatas : register(u1);

Texture2D<float32_t4> gCourseTexture : register(t2);

bool CollisionCheck(PolygonData polygonData, Segment segment)
{
    
    float32_t3 v01 = polygonData.positions1 - polygonData.positions0;
    float32_t3 v12 = polygonData.positions2 - polygonData.positions1;
    float32_t3 v20 = polygonData.positions0 - polygonData.positions2;

	//平面を求める
    float32_t3 planeNormal = normalize(cross(v01, v12));
    float32_t planeDistance = dot(polygonData.positions0, planeNormal);

	//平面との衝突確認
	//垂直判定のため、法線と線の内積を求める
    float dotValue = dot(planeNormal, segment.diff);

	//衝突していない
    if (dotValue == 0.0f)
    {
        return false;
    }

	//tを求める
    float t = (planeDistance - dot(segment.origin, planeNormal)) / dotValue;
    
    float32_t3 v1p = segment.origin + t * segment.diff - polygonData.positions1;
    float32_t3 v2p = segment.origin + t * segment.diff - polygonData.positions2;
    float32_t3 v0p = segment.origin + t * segment.diff - polygonData.positions0;

    float32_t3 cross01 = cross(v01, v1p);
    float32_t3 cross12 = cross(v12, v2p);
    float32_t3 cross20 = cross(v20, v0p);

	//三角形との衝突確認
    if (dot(cross01, planeNormal) >= 0.0f &&
		dot(cross12, planeNormal) >= 0.0f &&
		dot(cross20, planeNormal) >= 0.0f)
    {
        return true;
    }

    return false;
    
}

float32_t3 Extrusion(ObjectData objectData, PolygonData polygonData, uint32_t index)
{
    //平面とObb
    
    //平面を求める
    float32_t3 v01 = polygonData.positions1 - polygonData.positions0;
    float32_t3 v12 = polygonData.positions2 - polygonData.positions1;
    float32_t3 v20 = polygonData.positions0 - polygonData.positions2;
    
    float32_t3 planeNormal = normalize(cross(v01, v12));
    float32_t planeDistance = dot(polygonData.positions0, planeNormal);
    
    float32_t r = 0.0f;

    r += abs(dot(objectData.otientatuonsX * objectData.size.x, planeNormal));
    r += abs(dot(objectData.otientatuonsY * objectData.size.y, planeNormal));
    r += abs(dot(objectData.otientatuonsZ * objectData.size.z, planeNormal));

	//平面とobbの距離(怪しい)
    float32_t3 planePos = planeNormal * planeDistance;

    float32_t s = dot(objectData.center - planePos, planeNormal);
    float32_t distance = 0.0f;
    if (s > 0.0f)
    {
        distance = r - abs(s);
    }
    else
    {
        distance = r + abs(s);
    }
    
    // 衝突確認
    // ここオリジン値変更するかも
    Segment segment;
    segment.origin = objectData.center;
    segment.diff = planeNormal * distance;
    
    	// 頂点求める
    float32_t3 vertices[8];

	// 左 上 前
    vertices[0] = (
        (-objectData.size.x * objectData.otientatuonsX.x + objectData.size.y * objectData.otientatuonsY.x +
			-objectData.size.z * objectData.otientatuonsZ.x),
		(-objectData.size.x * objectData.otientatuonsX.y + objectData.size.y * objectData.otientatuonsY.y +
			-objectData.size.z * objectData.otientatuonsZ.y),
		(-objectData.size.x * objectData.otientatuonsX.z + objectData.size.y * objectData.otientatuonsY.z +
			-objectData.size.z * objectData.otientatuonsZ.z)
	);

	// 左 上 後
    vertices[1] = (
        (-objectData.size.x * objectData.otientatuonsX.x + objectData.size.y * objectData.otientatuonsY.x +
			objectData.size.z * objectData.otientatuonsZ.x),
		(-objectData.size.x * objectData.otientatuonsX.y + objectData.size.y * objectData.otientatuonsY.y +
			objectData.size.z * objectData.otientatuonsZ.y),
		(-objectData.size.x * objectData.otientatuonsX.z + objectData.size.y * objectData.otientatuonsY.z +
			objectData.size.z * objectData.otientatuonsZ.z)
	);

	// 右 上 前
    vertices[2] = (
        (objectData.size.x * objectData.otientatuonsX.x + objectData.size.y * objectData.otientatuonsY.x +
			-objectData.size.z * objectData.otientatuonsZ.x),
		(objectData.size.x * objectData.otientatuonsX.y + objectData.size.y * objectData.otientatuonsY.y +
			-objectData.size.z * objectData.otientatuonsZ.y),
		(objectData.size.x * objectData.otientatuonsX.z + objectData.size.y * objectData.otientatuonsY.z +
			-objectData.size.z * objectData.otientatuonsZ.z)
	);

	// 右 上 後
    vertices[3] = (
        (objectData.size.x * objectData.otientatuonsX.x + objectData.size.y * objectData.otientatuonsY.x +
			objectData.size.z * objectData.otientatuonsZ.x),
		(objectData.size.x * objectData.otientatuonsX.y + objectData.size.y * objectData.otientatuonsY.y +
			objectData.size.z * objectData.otientatuonsZ.y),
		(objectData.size.x * objectData.otientatuonsX.z + objectData.size.y * objectData.otientatuonsY.z +
			objectData.size.z * objectData.otientatuonsZ.z)
	);

	// 左 下 前
    vertices[4] = (
        (-objectData.size.x * objectData.otientatuonsX.x + -objectData.size.y * objectData.otientatuonsY.x +
			-objectData.size.z * objectData.otientatuonsZ.x),
		(-objectData.size.x * objectData.otientatuonsX.y + -objectData.size.y * objectData.otientatuonsY.y +
			-objectData.size.z * objectData.otientatuonsZ.y),
		(-objectData.size.x * objectData.otientatuonsX.z + -objectData.size.y * objectData.otientatuonsY.z +
			-objectData.size.z * objectData.otientatuonsZ.z)
	);

	// 左 下 後
    vertices[5] = (
        (-objectData.size.x * objectData.otientatuonsX.x + -objectData.size.y * objectData.otientatuonsY.x +
			objectData.size.z * objectData.otientatuonsZ.x),
		(-objectData.size.x * objectData.otientatuonsX.y + -objectData.size.y * objectData.otientatuonsY.y +
			objectData.size.z * objectData.otientatuonsZ.y),
		(-objectData.size.x * objectData.otientatuonsX.z + -objectData.size.y * objectData.otientatuonsY.z +
			objectData.size.z * objectData.otientatuonsZ.z)
	);

	// 右 下 前
    vertices[6] = (
        (objectData.size.x * objectData.otientatuonsX.x + -objectData.size.y * objectData.otientatuonsY.x +
			-objectData.size.z * objectData.otientatuonsZ.x),
		(objectData.size.x * objectData.otientatuonsX.y + -objectData.size.y * objectData.otientatuonsY.y +
			-objectData.size.z * objectData.otientatuonsZ.y),
		(objectData.size.x * objectData.otientatuonsX.z + -objectData.size.y * objectData.otientatuonsY.z +
			-objectData.size.z * objectData.otientatuonsZ.z)
	);

	// 右 下 後
    vertices[7] = (
         (objectData.size.x * objectData.otientatuonsX.x + -objectData.size.y * objectData.otientatuonsY.x +
			objectData.size.z * objectData.otientatuonsZ.x),
		(objectData.size.x * objectData.otientatuonsX.y + -objectData.size.y * objectData.otientatuonsY.y +
			objectData.size.z * objectData.otientatuonsZ.y),
		(objectData.size.x * objectData.otientatuonsX.z + -objectData.size.y * objectData.otientatuonsY.z +
			objectData.size.z * objectData.otientatuonsZ.z)
	);
    
    bool collisionCheck = false;
    
    for (uint32_t i = 0; i < 8; ++i)
    {   
        segment.origin = objectData.center + vertices[i];
        collisionCheck = CollisionCheck(polygonData, segment);
        if (collisionCheck)
        {
            break;
        }
        
    }
    
    if (abs(s) - r < 0.0f && collisionCheck)
    {
        return planeNormal * distance;
    }
    else
    {
        return float32_t3(0.0f, 0.0f, 0.0f);
    }

}

uint32_t SetDrivingLocation(float32_t2 texcoord)
{
    
    
    uint x = 0;
    uint y = 0;
    gCourseTexture.GetDimensions(x, y);
    
    float32_t2 newTexcoord = float32_t2(texcoord.x * x, texcoord.y * y);
    
    float32_t4 color = gCourseTexture[newTexcoord];
    
    if (color.r == 0.0f)
    {
        // 黒ければ道
        return 0;
    }
 
    // 白ければ壁
    return 2;
    
}

[numthreads(1024, 1, 1)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{
    
    // 番号
    uint32_t index = dispatchId.x;
    
    // オブジェクトデータ
    ObjectData objectData = gObjectData;
    // やらなくていいときにリターン
    if (index >= objectData.indexMax)
    {
        gOutputDatas[index].extrusion = float32_t3(0.0f, 0.0f, 0.0f);
        gOutputDatas[index].drivingLocation = 0;
        gOutputDatas[index].collided = 0;
        return;
    }
    
    // ポリゴンデータ
    PolygonData polygonData = gPolygonDatas[index];
    
    // 衝突確認 OBBの線分と三角ポリゴン
    
    float32_t3 result = Extrusion(objectData, polygonData, index);
    
    if (result.x == 0.0f && result.y == 0.0f && result.z == 0.0f)
    {
        gOutputDatas[index].extrusion = float32_t3(0.0f, 0.0f, 0.0f);
        gOutputDatas[index].collided = 0;
    }
    else
    {
        gOutputDatas[index].extrusion = result;
        gOutputDatas[index].collided = 1;
    }
    
    // 
    gOutputDatas[index].drivingLocation = SetDrivingLocation(polygonData.texcoord);

}