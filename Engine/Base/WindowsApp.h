#pragma once
#include <windows.h>
#include <cstdint>

class WindowsApp
{
private:
	//ウィンドウクラスの設定
	WNDCLASSEX w_{};
	HWND hwnd_ = nullptr;

	//ウィンドウサイズ
	const int32_t WINDOW_WIDTH = 1280;
	const int32_t WINDOW_HEIGHT = 720;

	WindowsApp() = default;
	~WindowsApp() = default;
	//コピーコンストラクタ無効
	WindowsApp(const WindowsApp& obj) = delete;
	//代入演算子を無効
	WindowsApp& operator=(const WindowsApp& obj) = delete;

public:	//静的メンバ関数

	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static WindowsApp* GetInstance();

public:	//メンバ関数

	// 初期化
	void Initalize();

	// ゲームループ終了メッセージ取得
	bool gameloopExit(MSG& msg);

	// 登録解除
	void Release();

	//getter
	HWND GetHwnd() const { return hwnd_; }
	WNDCLASSEX GetW() const { return w_; };
	HINSTANCE GetHInstancee() const { return w_.hInstance; };
	const int32_t GetWindowWidth() { return WINDOW_WIDTH; };
	const int32_t GetWindowHeight() { return WINDOW_HEIGHT; };
};

