#pragma once
#include "Sound.h"

class SoundManager
{
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

