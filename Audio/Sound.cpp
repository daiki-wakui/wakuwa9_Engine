#include "Sound.h"

#include <cassert>
#include <fstream>

#pragma comment (lib,"xaudio2.lib")

void Sound::Initialize(const std::string& directoryPath)
{
	this->directoryPath_ = directoryPath;

	IXAudio2MasteringVoice* masterVoice;

	HRESULT result = S_FALSE;
	//オーディオ初期化
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	//マスターボイス生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

}

void Sound::Finalize()
{
	xAudio2_.Reset();

	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();



	for (; it != soundDatas_.end(); ++it) {
		Unload(&it->second);
	}

	soundDatas_.clear();
	//SoundUnload(&soundData1);
	//SoundUnload(&soundData2);
}

void Sound::LoadWave(const std::string& filename)
{

	if (soundDatas_.find(filename) != soundDatas_.end()) {
		return;
	}

	std::string fullpath = directoryPath_ + filename;

	//ファイルオープン
	//ファイル入力ストリームのインスタンス
	std::ifstream file;

	//.wavファイルをバイナリモードで開く
	file.open(fullpath, std::ios_base::binary);
	//ファイルオープン失敗をチェック
	assert(file.is_open());

	//wav読み込み
	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	/*if (strncmp(format.chunk.id, "fmt", 4) != 0) {
		assert(0);
	}*/
	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//ファイルクローズ
	file.close();

	//読み込んだ音声データをretrun
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	soundDatas_.insert(std::make_pair(filename, soundData));
}

void Sound::Unload(SoundData* soundData)
{
	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Sound::PlayWave(const std::string& filename)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	assert(it != soundDatas_.end());

	SoundData& soundData = it->second;

	//波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSoundVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSoundVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	soundData.pSoundVoice_ = pSoundVoice;

	//波形データの再生
	result = pSoundVoice->SubmitSourceBuffer(&buf);
	result = pSoundVoice->Start();
}

void Sound::StopWAVE(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	assert(it != soundDatas_.end());

	SoundData& soundData = it->second;

	soundData.pSoundVoice_->Stop();
}
