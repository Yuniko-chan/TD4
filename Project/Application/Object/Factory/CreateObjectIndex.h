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

	// 地形
	kCreateObjectIndexTerrain,

	kCreateObjectIndexOfCount, // 数数える用
};