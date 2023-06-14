#pragma once
#include "Sprite.h"
#include <cstdint>

class PostEffect :  public Sprite
{
public:

	//コンストラクタ
	PostEffect();

	//描画
	void Draw(int32_t texNum);

};

