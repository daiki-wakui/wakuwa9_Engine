#include "SoundManager.h"
#include "KeyBoard.h"

//初期化
void SoundManager::Initialize()
{
	//BGM
	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("NieR_Title.wav");
	sound_->LoadWave("NieR_freld2.wav");
	sound_->LoadWave("NieR_Field.wav");
	sound_->LoadWave("Alone.wav");
	sound_->LoadWave("NieR_boss.wav");
	//SE
	sound_->LoadWave("Start.wav");
	sound_->LoadWave("Shot.wav");
	sound_->LoadWave("Hit.wav");
	sound_->LoadWave("noise.wav");
	sound_->LoadWave("Warning.wav");
	sound_->LoadWave("electric_shock3.wav");
}

//更新処理
void SoundManager::Update(int32_t state)
{
	if (isFiledBGM_) {
		filedVolue_ += 0.01f;
		filedVolue_ = min(1.0f, filedVolue_);
	}

	if (state == 0) {

		if (!titleBGM_) {
			sound_->PlayLoopWave("NieR_Title.wav", 0.75f);
			titleBGM_ = true;
		}
	}

	if (state == 1) {
		if (!gameBGM_ && !bossBGM_) {
			sound_->PlayLoopWave("NieR_Field.wav", 0.25f);
			sound_->PlayLoopWave("NieR_freld2.wav", 0.25f);
			gameBGM_ = true;
		}

		sound_->SetVolue("NieR_Field.wav", (1.0f - filedVolue_) / 4);
		sound_->SetVolue("NieR_freld2.wav", (0.0f + filedVolue_) / 4);
	}

	if (state == 2) {
		if (!bossBGM_) {
			sound_->PlayLoopWave("NieR_boss.wav", 0.4f);
			bossBGM_ = true;
		}
	}
}

//setter
void SoundManager::SetBasis(Sound* sound)
{
	sound_ = sound;
}

//SE鳴らす用
void SoundManager::PlayWave(const std::string& filename, float volue)
{
	sound_->PlayWave(filename, volue);
}

//シングルートン
SoundManager* SoundManager::GetInstance()
{
	static SoundManager instance;

	return &instance;
}

//BGMを止める
void SoundManager::StopBGM()
{
	

	if (titleBGM_) {
		sound_->StopWAVE("NieR_Title.wav");
		titleBGM_ = false;
	}

	if (gameBGM_) {
		sound_->StopWAVE("NieR_Field.wav");
		sound_->StopWAVE("NieR_freld2.wav");
		gameBGM_ = false;
	}

	if (bossBGM_) {
		sound_->StopWAVE("NieR_boss.wav");
		bossBGM_ = false;
	}
}
