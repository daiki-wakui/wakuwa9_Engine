#pragma once
#include "Sound.h"

class SoundManager
{
public:
	//������
	void Initialize();

	//���t���[��
	void Update(int32_t state);

	void SetBasis(Sound* sound_);

	static SoundManager* GetInstance();

	void SetBossBGM(bool isBossBGM) { isBoss_ = isBossBGM; }

private:

	SoundManager() = default;
	~SoundManager() = default;
	//�R�s�[�R���X�g���N�^����
	SoundManager(const SoundManager& obj) = delete;
	//������Z�q�𖳌�
	SoundManager& operator=(const SoundManager& obj) = delete;

	

	Sound* sound_ = nullptr;

	//BGM
	bool titleBGM_ = false;
	bool gameBGM_ = false;
	bool bossBGM_ = false;
	bool isBoss_ = false;
};

