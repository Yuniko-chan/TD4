#include "CourseCollisionSystem.h"

void CourseCollisionSystem::Initialize()
{
}

void CourseCollisionSystem::Execute()
{
	
	// 登録分回す
	for (std::list<MeshObject*>::iterator itr = collidingObjects_.begin();
		itr != collidingObjects_.end();++itr) {
		// CPU側でOBBとの距離判定をとる->メッシュ登録
		DistanceJudgment(*itr);

		// ->GPU側で押し出し距離確認


	}

	// ->キック

	// 登録分回す
	for (std::list<MeshObject*>::iterator itr = collidingObjects_.begin();
		itr != collidingObjects_.end(); ++itr) {
	
		// ->CPU側で押し出し、回転（壁データはとらない）
		// ->OBB登録のオブジェクトのワールドトランスフォーム更新
	
	}

	// 後処理

	// 登録したオブジェクトリストをクリア
	collidingObjects_.clear();

}

void CourseCollisionSystem::ObjectRegistration(MeshObject* object)
{

	// オブジェクトリストに登録
	collidingObjects_.push_back(object);

}

void CourseCollisionSystem::DistanceJudgment(MeshObject* object)
{


	object;
	// コライダー取得

	// コースメッシュ分回す

		// 距離をはかる

		// 距離が近ければメッシュ登録


}
