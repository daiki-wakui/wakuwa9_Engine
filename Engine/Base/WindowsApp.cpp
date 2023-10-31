#include "WindowsApp.h"
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//ウィンドウプロシージャ
LRESULT WindowsApp::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//Imgui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return true;

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
			//ウィンドウが破棄された
		case WM_DESTROY:
			//OSに対して、アプリの終了を伝える
			PostQuitMessage(0);
			return 0;
	}
	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsApp* WindowsApp::GetInstance()
{
	static WindowsApp instance;

	return &instance;
}

//初期化
void WindowsApp::Initalize()
{

	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)windowProc;	//ウィンドウプロシージャを設定
	w_.lpszClassName = L"DirectXGame";	//ウィンドウクラス名
	w_.hInstance = GetModuleHandle(nullptr);	//ウィンドウハンドル
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソル設定

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w_);
	//ウィンドウサイズ{X,Y,横,縦}
	RECT wrc = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	//自動でサイズを補正する
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

//ゲームループ終了判定
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

//登録解除
void WindowsApp::Release(){
	//ウィンドウクラスを登録解除
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}


