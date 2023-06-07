#include "GameCore.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GameCore game;

	game.Initialize();
	MSG msg{};

	OutputDebugStringA("�����񃊃e�������o�͂����\n");

	std::string a("string�ɖ��ߍ��񂾕�������擾�����\n");
	OutputDebugStringA(a.c_str());

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (game.GetWindows()->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}

		game.Update();

		game.Draw();
	}

	game.Finalize();

	return 0;
}
