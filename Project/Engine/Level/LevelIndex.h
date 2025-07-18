#pragma once

/// <summary>
/// シーンの種類
/// </summary>
enum LevelIndex {
	kLevelIndexTitle, // タイトル
	kLevelIndexTutorial, // チュートリアル
	kLevelIndexMain, // メイン

	kLevelIndexGenerationPattern_00, // 生成パターン_00
	kLevelIndexGenerationPattern_01, // 生成パターン_01

	kLevelIndexDebug,//テスト

	kLevelIndexCourseA,//コース形状に紐づいたギミック群
	kLevelIndexCourseB,
	kLevelIndexCourseC,

	kLevelIndexOfCount // 数を数える用
};