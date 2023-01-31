#include "Sound.h"

#include <cassert>
#include <fstream>

#pragma comment (lib,"xaudio2.lib")

void Sound::Initialize(const std::string& directoryPath)
{
	this->directoryPath = directoryPath;

	IXAudio2MasteringVoice* masterVoice;

	HRESULT result = S_FALSE;
	//�I�[�f�B�I������
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	//�}�X�^�[�{�C�X����
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

}

void Sound::Finalize()
{
	xAudio2.Reset();

	std::map<std::string, SoundData>::iterator it = soundDatas.begin();



	for (; it != soundDatas.end(); ++it) {
		Unload(&it->second);
	}

	soundDatas.clear();
	//SoundUnload(&soundData1);
	//SoundUnload(&soundData2);
}

void Sound::LoadWave(const std::string& filename)
{
	HRESULT result;

	if (soundDatas.find(filename) != soundDatas.end()) {
		return;
	}

	std::string fullpath = directoryPath + filename;

	//�t�@�C���I�[�v��
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fullpath, std::ios_base::binary);
	//�t�@�C���I�[�v�����s���`�F�b�N
	assert(file.is_open());

	//wav�ǂݍ���
	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	/*if (strncmp(format.chunk.id, "fmt", 4) != 0) {
		assert(0);
	}*/
	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//�ǂݍ��݈ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//�t�@�C���N���[�Y
	file.close();

	//�ǂݍ��񂾉����f�[�^��retrun
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	soundDatas.insert(std::make_pair(filename, soundData));
}

void Sound::Unload(SoundData* soundData)
{
	//�o�b�t�@�̃����������
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Sound::PlayWave(const std::string& filename)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);
	assert(it != soundDatas.end());

	SoundData& soundData = it->second;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice* pSoundVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSoundVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	soundData.pSoundVoice_ = pSoundVoice;

	//�g�`�f�[�^�̍Đ�
	result = pSoundVoice->SubmitSourceBuffer(&buf);
	result = pSoundVoice->Start();
}

void Sound::StopWAVE(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);
	assert(it != soundDatas.end());

	SoundData& soundData = it->second;

	soundData.pSoundVoice_->Stop();
}
