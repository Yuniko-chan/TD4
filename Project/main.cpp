#include <Windows.h>
#include "Engine/Framework/MyFramework.h"
#include "Application/Framework/MyGame.h"

using namespace NomeEngine;

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	/// ディスコ通知確認

	MyFramework* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}
