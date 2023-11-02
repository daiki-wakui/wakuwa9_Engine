#pragma once
#include "SpriteBasis.h"

class TextureManager
{
private:	//シングルートン

	TextureManager() = default;
	~TextureManager() = default;
	//コピーコンストラクタ無効
	TextureManager(const TextureManager& obj) = delete;
	//代入演算子を無効
	TextureManager& operator = (const TextureManager& obj) = delete;

protected:

	//画像
	int32_t titleImage_ = 0;
	int32_t titleUIImage_ = 0;
	int32_t sceneChangeImage_ = 0;
	int32_t filterImage_ = 0;

	int32_t testImage_ = 0;

	SpriteBasis* spBasis_ = SpriteBasis::GetInstance();

public:

	static TextureManager* GetInstance();

	void LoadTexture();

};

