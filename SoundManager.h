#pragma once
#include "Sound.h"

class SoundManager
{
public:
	//初期化
	void Initialize();

	//毎フレーム
	void Update(int32_t state);

	void SetBasis(Sound* sound_);

	static SoundManager* GetInstance();

	void SetBossBGM(bool isBossBGM) { isBoss_ = isBossBGM; }

private:

	SoundManager() = default;
	~SoundManager() = default;
	//コピーコンストラクタ無効
	SoundManager(const SoundManager& obj) = delete;
	//代入演算子を無効
	SoundManager& operator=(const SoundManager& obj) = delete;

	

	Sound* sound_ = nullptr;

	//BGM
	bool titleBGM_ = false;
	bool gameBGM_ = false;
	bool bossBGM_ = false;
	bool isBoss_ = false;
};

