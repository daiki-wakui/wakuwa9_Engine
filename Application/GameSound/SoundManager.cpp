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
		filedVolue_ += ADD_FILED_VOLUE;
		filedVolue_ = min(MAX_FILED_VOLUE, filedVolue_);
	}

	if (state == TITLE_BGM) {

		if (!titleBGM_) {
			sound_->PlayLoopWave("NieR_Title.wav", TITLE_BGM_VOLUE);
			titleBGM_ = true;
		}
	}

	if (state == GAME_BGM) {
		if (!gameBGM_ && !bossBGM_) {
			sound_->PlayLoopWave("NieR_Field.wav", GAME_BGM_VOLUE);
			sound_->PlayLoopWave("NieR_freld2.wav", GAME_BGM_VOLUE);
			gameBGM_ = true;
		}

		sound_->SetVolue("NieR_Field.wav", (MAX_VLOUE - filedVolue_) / GAME_BGM_VOLUE_RATE);
		sound_->SetVolue("NieR_freld2.wav", (MIN_VLOUE + filedVolue_) / GAME_BGM_VOLUE_RATE);
	}

	if (state == BOSS_BGM) {
		if (!bossBGM_) {
			sound_->PlayLoopWave("NieR_boss.wav", BOSS_BGM_VOLUE);
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
