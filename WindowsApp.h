#pragma once
#include <windows.h>

class WindowsApp
{
private:
	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	HWND hwnd = nullptr;

public:	//�ÓI�����o�֐�

	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:	//�����o�֐�

	//������
	void Initalize();

	//�X�V
	void Update();

	void Release();
	bool gameloopExit(MSG& msg);

	//getter
	HWND GetHwnd() const { return hwnd; }
};

