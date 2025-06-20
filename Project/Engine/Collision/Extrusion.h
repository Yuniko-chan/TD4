#pragma once
#include "../Collider/OBB/OBB.h"
#include "../Collider/Plane/Plane.h"
#include "../Line/Segment.h"

/// <summary>
/// 押し出し処理
/// </summary>
class Extrusion
{

public: // メンバ関数

	/// <summary>
	/// OBBとOBBの押し出し
	/// </summary>
	/// <param name="pushedOut">押し出される</param>
	/// <param name="pushOut">押し出す</param>
	/// <returns>押し出し距離</returns>
	static Vector3 OBBAndOBB(OBB* pushedOut, OBB* pushOut);

	/// <summary>
	/// OBBとPlaneの押し出し
	/// </summary>
	/// <param name="pushedOut">押し出される</param>
	/// <param name="pushOut">押し出す</param>
	/// <returns></returns>
	static Vector3 OBBAndPlane(OBB* pushedOut, Plane* pushOut, Vector3 planeVertices[4]);

private:

	static bool OBBAndPlaneCollisionCheck(Vector3 planeVertices[3], const Segment& segment, const Vector3& planeNormal,const float& planeDistance);

};

