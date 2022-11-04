#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>

using namespace DirectX;

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define DIRECTINPUT_VERSION	0x0800


class KeyBoard
{
private:

public:
	void Initialize();
	void Update();

	bool keyPush(uint8_t key);
	bool keyRelease(uint8_t key);
	bool keyInstantPush(uint8_t key);
	bool keyInstantRelease(uint8_t key);

};

