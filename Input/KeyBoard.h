#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//#define DIRECTINPUT_VERSION	0x0800

using namespace DirectX;

class KeyBoard
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
	BYTE key_[256] = {};
	BYTE oldkey_[256] = {};

public:
	void Initialize(HINSTANCE hInstance, HWND hwnd);
	void Update();

	bool keyPush(uint8_t key);
	bool keyRelease(uint8_t key);
	bool keyInstantPush(uint8_t key);
	bool keyInstantRelease(uint8_t key);

};

