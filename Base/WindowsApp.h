#pragma once
#include <windows.h>

class WindowsApp
{
private:
	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	HWND hwnd = nullptr;

	//�E�B���h�E�T�C�Y
	const int window_width = 1280;
	const int window_height = 720;

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
	HWND GetHwnd() const { return hwnd; }
	WNDCLASSEX GetW() const { return w; };
	HINSTANCE GetHInstancee() const { return w.hInstance; };
	const int GetWindowWidth() { return window_width; };
	const int GetWindowHeight() { return window_height; };
};

