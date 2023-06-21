#pragma once
#include "Sprite.h"
#include <cstdint>

class PostEffect :  public Sprite
{
private:

	ComPtr<ID3D12Resource> texBuff_;

	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	SpriteBasis* spBasis_;
	WindowsApp* winApp_;

public:

	void Initialize();

	void SetDirectX(SpriteBasis* spBasis, WindowsApp* winApp);

	//コンストラクタ
	PostEffect();

	//描画
	void Draw(int32_t texNum);

};

