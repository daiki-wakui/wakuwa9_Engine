#pragma once
#include "SpriteBasis.h"

class TextureManager
{
protected:

	//画像
	int32_t titleImage_ = 0;
	int32_t titleUIImage_ = 0;
	int32_t sceneChangeImage_ = 0;
	int32_t filterImage_ = 0;

	int32_t testImage_ = 0;

	SpriteBasis* spBasis_ = SpriteBasis::GetInstance();

public:

	TextureManager();
	~TextureManager();

	void LoadTexture();

};

