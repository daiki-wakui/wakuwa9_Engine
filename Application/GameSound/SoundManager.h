#pragma once
#include "Sound.h"

/**
 * @file SoundManager
 * @brief BGMやSEを管理しているクラス
 */

enum BGMpattern
{
	TITLE_BGM,
	GAME_BGM,
	BOSS_BGM,
};

class SoundManager
{
private: //定数

	const float ADD_FILED_VOLUE = 0.01f;
	const float MAX_FILED_VOLUE = 1.0f;

	const float TITLE_BGM_VOLUE = 0.75f;
	const float GAME_BGM_VOLUE = 0.25f;
	const float GAME_BGM_VOLUE_RATE = 4.0f;
	const float MAX_VLOUE = 1.0f;
	const float MIN_VLOUE = 0.0f;
	const float BOSS_BGM_VOLUE = 0.4f;

public:
	//インスタンス
	static SoundManager* GetInstance();

	//初期化
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="state"></param> どのシーンかで流すBGMを決める
	void Update(int32_t state);

	//SEを鳴らす用の関数
	void PlayWave(const std::string& filename, float volue);

	//鳴っているBGM
	void StopBGM();

	//setter
	void SetBasis(Sound* sound_);
	void SetBossBGM(bool isBossBGM) { isBoss_ = isBossBGM; }
	void SetFiledBGM(bool isFiledBGM) { isFiledBGM_ = isFiledBGM; }
	void SetVolue(float volue) { filedVolue_ = volue; }

private:
	SoundManager() = default;
	~SoundManager() = default;
	//コピーコンストラクタ無効
	SoundManager(const SoundManager& obj) = delete;
	//代入演算子を無効
	SoundManager& operator=(const SoundManager& obj) = delete;

	Sound* sound_ = nullptr;

	bool isFiledBGM_ = false;
	float filedVolue_ = 0.0f;

	//BGM
	bool titleBGM_ = false;
	bool gameBGM_ = false;
	bool bossBGM_ = false;
	bool isBoss_ = false;
};

