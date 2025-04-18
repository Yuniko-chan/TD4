#pragma once

/// <summary>
/// オブジェクト作成用の番号
/// </summary>

enum CreateObjectIndex {
	kCreateObjectIndexSkydome, // スカイドーム
	kCreateObjectIndexPlayer,	// プレイヤー
	// 車両関係
	kCreateObjectIndexCarCore,	// コア
	kCreateObjectIndexCarEngine,	// エンジン
	kCreateObjectIndexCarTire,	// タイヤ
	kCreateObjectIndexCarFrame,	// フレーム

	kCreateObjectIndexOfCount, // 数数える用
};