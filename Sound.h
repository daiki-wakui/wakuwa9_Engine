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

	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];	//チャンクごとのid
		int32_t size;	//チャンクサイズ
	};

	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;	//RIFF
		char type[4];	//WAVE
	};

	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;	//fmt
		WAVEFORMATEX fmt;	//波形フォーマット
	};

	//音声データ
	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭アドレス
		BYTE* pBuffer;
		//バッファのサイズ
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
	//サウンドデータの連想配列
	std::map<std::string, SoundData> soundDatas;

	std::string directoryPath;
};

