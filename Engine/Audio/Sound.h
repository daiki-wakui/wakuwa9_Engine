#pragma once

#include <xaudio2.h>
#include <cstdint>
#include <wrl.h>
#include <map>
#include <string>
#include <cstdint>
#include <stdint.h>

class Sound
{
private:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id_[4];	//�`�����N���Ƃ�id
		int32_t size_;	//�`�����N�T�C�Y
	};

	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk_;	//RIFF
		char type_[4];	//WAVE
	};

	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk_;	//fmt
		WAVEFORMATEX fmt_;	//�g�`�t�H�[�}�b�g
	};

	//�����f�[�^
	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex_;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer_;
		//�o�b�t�@�̃T�C�Y
		uint32_t bufferSize_;

		IXAudio2SourceVoice* pSoundVoice_;
	};

	void Initialize(const std::string& directoryPath = "Resources/Sound/");

	void Finalize();

	void LoadWave(const std::string& filename);

	void Unload(SoundData* soundData);

	void PlayWave(const std::string& filename, float volue = 1.0f);
	void PlayLoopWave(const std::string& filename, float volue = 1.0f);


	void StopWAVE(const std::string& filename);

private:

	ComPtr<IXAudio2> xAudio2_;
	//�T�E���h�f�[�^�̘A�z�z��
	std::map<std::string, SoundData> soundDatas_;

	std::string directoryPath_;
};

