#pragma once
#include "SpriteBasis.h"

/**
 * @file TextureManager
 * @brief ゲームで使う2Dスプライトの読み込みクラス
 */

class TextureManager
{
protected:

	//画像
	int32_t titleImage_ = 0;
	int32_t titleUIImage_ = 0;
	int32_t sceneChangeImage_ = 0;
	int32_t filterImage_ = 0;
	int32_t stepFilterImage_ = 0;
	int32_t testImage_ = 0;


	//HP
	int32_t playerHP_ = 0;
	int32_t bossHP_ = 0;
	int32_t bossMaxHP_ = 0;
	int32_t bossHPImage_ = 0;

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

	//コンストラクタ、デストラクタ
	TextureManager();
	~TextureManager();

	//タイトルシーンで使うスプライト読み込み
	void LoadTitleTexture();

	//ゲームシーンで使うスプライト読み込み
	void LoadGameTexture();

};

