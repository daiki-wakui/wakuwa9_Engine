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


	//HP
	int32_t playerHP_ = 0;
	int32_t bossHP_ = 0;

	//タイトル
	int32_t gameover_ = 0;
	int32_t gameclear_ = 0;


	int32_t reticleImage_ = 0;
	int32_t tesImage_ = 0;
	int32_t targetImage_ = 0;
	int32_t iventImage_ = 0;
	int32_t warningImage_ = 0;
	int32_t manualImageRB_ = 0;
	int32_t exImage_ = 0;
	int32_t damageFilter_ = 0;
	int32_t bulletRreticleImage_ = 0;


	SpriteBasis* spBasis_ = SpriteBasis::GetInstance();

public:

	TextureManager();
	~TextureManager();

	void LoadTexture();

};

