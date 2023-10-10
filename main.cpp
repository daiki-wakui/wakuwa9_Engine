#include "GameCore.h"
#include "Framework.h"
#include <memory>

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {

	std::unique_ptr<Framework> game_ = std::make_unique<GameCore>();

	//ゲームループ
	game_->Run();

	return 0;
}
