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

	//チャンクヘッダ
	struct ChunkHeader
	{
		char id_[4];	//チャンクごとのid
		int32_t size_;	//チャンクサイズ
	};

	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk_;	//RIFF
		char type_[4];	//WAVE
	};

	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk_;	//fmt
		WAVEFORMATEX fmt_;	//波形フォーマット
	};

	//音声データ
	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex_;
		//バッファの先頭アドレス
		BYTE* pBuffer_;
		//バッファのサイズ
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
	//サウンドデータの連想配列
	std::map<std::string, SoundData> soundDatas_;

	std::string directoryPath_;
};

