#pragma once

#include <xaudio2.h>
#include <cstdint>
#include <wrl.h>
#include <map>
#include <string>

class Sound
{
private:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];	//�`�����N���Ƃ�id
		int32_t size;	//�`�����N�T�C�Y
	};

	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;	//RIFF
		char type[4];	//WAVE
	};

	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;	//fmt
		WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
	};

	//�����f�[�^
	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;
		//�o�b�t�@�̃T�C�Y
		unsigned int bufferSize;

		IXAudio2SourceVoice* pSoundVoice_;
	};

	void Initialize(const std::string& directoryPath = "Resources/");

	void Finalize();

	void LoadWave(const std::string& filename);

	void Unload(SoundData* soundData);

	void PlayWave(const std::string& filename);

	void StopWAVE(const std::string& filename);

private:

	ComPtr<IXAudio2> xAudio2;
	//�T�E���h�f�[�^�̘A�z�z��
	std::map<std::string, SoundData> soundDatas;

	std::string directoryPath;
};

