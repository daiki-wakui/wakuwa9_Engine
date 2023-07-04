#include "GameCore.h"

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {

	GameCore game;

	game.Initialize();
	MSG msg{};

	OutputDebugStringA("文字列リテラルを出力するよ\n");

	std::string a("stringに埋め込んだ文字列を取得するよ\n");
	OutputDebugStringA(a.c_str());

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (game.GetWindows()->gameloopExit(msg) == true || game.GetInput()->keyInstantPush(DIK_ESCAPE) == true) {
			break;	//ゲームループ終了
		}

		game.Update();

		game.Draw();
	}

	game.Finalize();

	return 0;
}
