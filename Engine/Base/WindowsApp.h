#pragma once
#include <windows.h>
#include <cstdint>

class WindowsApp
{
private:
	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w_{};
	HWND hwnd_ = nullptr;

	//�E�B���h�E�T�C�Y
	const int32_t WINDOW_WIDTH = 1280;
	const int32_t WINDOW_HEIGHT = 720;

public:	//�ÓI�����o�֐�

	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:	//�����o�֐�

	// ������
	void Initalize();

	// �Q�[�����[�v�I�����b�Z�[�W�擾
	bool gameloopExit(MSG& msg);

	// �o�^����
	void Release();

	//getter
	HWND GetHwnd() const { return hwnd_; }
	WNDCLASSEX GetW() const { return w_; };
	HINSTANCE GetHInstancee() const { return w_.hInstance; };
	const int32_t GetWindowWidth() { return WINDOW_WIDTH; };
	const int32_t GetWindowHeight() { return WINDOW_HEIGHT; };
};

