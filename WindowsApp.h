#pragma once
#include <windows.h>

class WindowsApp
{
private:
	//ウィンドウクラスの設定
	WNDCLASSEX w{};
	HWND hwnd = nullptr;

	//ウィンドウサイズ
	const int window_width = 1280;
	const int window_height = 720;

public:	//静的メンバ関数

	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:	//メンバ関数

	// 初期化
	void Initalize();

	// ゲームループ終了メッセージ取得
	bool gameloopExit(MSG& msg);

	// 登録解除
	void Release();

	//getter
	HWND GetHwnd() const { return hwnd; }
	WNDCLASSEX GetW() const { return w; };
	HINSTANCE GetHInstancee() const { return w.hInstance; };
	const int GetWindowWidth() { return window_width; };
	const int GetWindowHeight() { return window_height; };
};

