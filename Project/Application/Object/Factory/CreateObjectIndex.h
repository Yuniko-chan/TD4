#pragma once

/// <summary>
/// オブジェクト作成用の番号
/// </summary>

enum CreateObjectIndex {
	kCreateObjectIndexSkydome, // スカイドーム
	kCreateObjectIndexPlayer,	// プレイヤー
	// 車両関係
	kCreateObjectIndexVehicleCore,	// 車両コア
	kCreateObjectIndexCarEngine,	// エンジン
	kCreateObjectIndexCarTire,	// タイヤ
	kCreateObjectIndexCarArmorFrame,	// フレーム

	kCreateObjectIndexWall,	// 壁
	kCreateObjectIndexCourse,//コース

	// 地形
	kCreateObjectIndexTerrain,

	///ギミック
	kCreateObjectIndexIronBall,
	kCreateObjectIndexCannon,


	kCreateObjectIndexOfCount, // 数数える用
};