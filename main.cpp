#include "GameCore.h"
#include "Framework.h"
#include <memory>

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {

	//GameCore game;

	std::unique_ptr<Framework> game_ = std::make_unique<GameCore>();

	/*game.Initialize();
	MSG msg{};*/

	game_->Run();

	//�Q�[�����[�v
	//while (true) {
	//	//�~�{�^���ŏI�����b�Z�[�W��������
	//	if (game.GetIsEnd()) {
	//		break;	//�Q�[�����[�v�I��
	//	}

	//	game.Update();

	//	game.Draw();
	//}

	//game.Finalize();

	return 0;
}
