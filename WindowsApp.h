#pragma once
#include <windows.h>

class WindowsApp
{
private:
	//ウィンドウクラスの設定
	WNDCLASSEX w{};
	HWND hwnd = nullptr;

public:	//静的メンバ関数

	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:	//メンバ関数

	//初期化
	void Initalize();

	//更新
	void Update();

	void Release();
	bool gameloopExit(MSG& msg);

	//getter
	HWND GetHwnd() const { return hwnd; }
};

