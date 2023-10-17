#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <wrl.h>

class KeyBoard
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
	BYTE key_[256] = {};
	BYTE oldkey_[256] = {};

	KeyBoard() = default;
	~KeyBoard() = default;
	//コピーコンストラクタ無効
	KeyBoard(const KeyBoard& obj) = delete;
	//代入演算子を無効
	KeyBoard& operator=(const KeyBoard& obj) = delete;

public:
	static KeyBoard* GetInstance();


	void Initialize(HINSTANCE hInstance, HWND hwnd);
	void Update();

	bool keyPush(uint8_t key);
	bool keyRelease(uint8_t key);
	bool keyInstantPush(uint8_t key);
	bool keyInstantRelease(uint8_t key);

};

