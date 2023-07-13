#include "GameCore.h"
#include "Framework.h"
#include <memory>

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {

	//GameCore game;

	std::unique_ptr<Framework> game_ = std::make_unique<GameCore>();

	/*game.Initialize();
	MSG msg{};*/

	game_->Run();

	//ゲームループ
	//while (true) {
	//	//×ボタンで終了メッセージがきたら
	//	if (game.GetIsEnd()) {
	//		break;	//ゲームループ終了
	//	}

	//	game.Update();

	//	game.Draw();
	//}

	//game.Finalize();

	return 0;
}
