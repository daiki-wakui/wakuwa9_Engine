#include "KeyBoard.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

using namespace DirectX;

void KeyBoard::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	//DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

}

void KeyBoard::Update()
{
	//キーボード情報の取得開始
	keyboard_->Acquire();

	for (int32_t i = 0; i < 256; i++) {
		oldkey_[i] = key_[i];
	}

	//全キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);

}

bool KeyBoard::keyPush(uint8_t key)
{
	return key_[key];
}

bool KeyBoard::keyRelease(uint8_t key)
{
	return !key_[key];
}

bool KeyBoard::keyInstantPush(uint8_t key)
{
	return key_[key] && !oldkey_[key];
}

bool KeyBoard::keyInstantRelease(uint8_t key)
{
	return !key_[key] && oldkey_[key];
}
