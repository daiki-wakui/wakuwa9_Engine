#include "SoundManager.h"

void SoundManager::Initialize()
{
	sound_->LoadWave("PerituneMaterial.wav");
	sound_->LoadWave("NieR_Title.wav");
	sound_->LoadWave("NieR_freld2.wav");
	sound_->LoadWave("Alone.wav");
	sound_->LoadWave("NieR_boss.wav");
}

void SoundManager::Update(int32_t state)
{
	if (state == 0) {

		if (!titleBGM_) {
			sound_->PlayLoopWave("NieR_Title.wav", 0.75f);
			titleBGM_ = true;
		}
	}

	if (state == 1) {
		if (!gameBGM_ && !bossBGM_) {
			sound_->PlayLoopWave("NieR_freld2.wav", 0.25f);
			gameBGM_ = true;
		}
	}

	if (state == 2) {
		if (!bossBGM_) {
			sound_->PlayLoopWave("NieR_boss.wav", 0.4f);
			bossBGM_ = true;
		}
	}
}

void SoundManager::SetBasis(Sound* sound)
{
	sound_ = sound;
}

SoundManager* SoundManager::GetInstance()
{
	static SoundManager instance;

	return &instance;
}

void SoundManager::StopBGM()
{
	if (titleBGM_) {
		sound_->StopWAVE("NieR_Title.wav");
		titleBGM_ = false;
	}

	if (gameBGM_) {
		sound_->StopWAVE("NieR_freld2.wav");
		gameBGM_ = false;
	}

	if (bossBGM_) {
		sound_->StopWAVE("NieR_boss.wav");
		bossBGM_ = false;
	}
}
