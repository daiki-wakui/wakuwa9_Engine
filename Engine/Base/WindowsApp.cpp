#include "WindowsApp.h"
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//�E�B���h�E�v���V�[�W��
LRESULT WindowsApp::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//Imgui�p�E�B���h�E�v���V�[�W���Ăяo��
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return true;

	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
			//�E�B���h�E���j�����ꂽ
		case WM_DESTROY:
			//OS�ɑ΂��āA�A�v���̏I����`����
			PostQuitMessage(0);
			return 0;
	}
	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//������
void WindowsApp::Initalize()
{

	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)windowProc;	//�E�B���h�E�v���V�[�W����ݒ�
	w_.lpszClassName = L"DirectXGame";	//�E�B���h�E�N���X��
	w_.hInstance = GetModuleHandle(nullptr);	//�E�B���h�E�n���h��
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���ݒ�

	//�E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w_);
	//�E�B���h�E�T�C�Y{X,Y,��,�c}
	RECT wrc = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	//�����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(w_.lpszClassName,
		L"DirectXGame",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w_.hInstance,
		nullptr
	);

	ShowWindow(hwnd_, SW_SHOW);
}

//�Q�[�����[�v�I������
bool WindowsApp::gameloopExit(MSG& msg){

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

//�o�^����
void WindowsApp::Release(){
	//�E�B���h�E�N���X��o�^����
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}


