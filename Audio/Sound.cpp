#include "Sound.h"

#include <cassert>
#include <fstream>

#pragma comment (lib,"xaudio2.lib")

void Sound::Initialize(const std::string& directoryPath)
{
	this->directoryPath_ = directoryPath;

	IXAudio2MasteringVoice* masterVoice;

	HRESULT result = S_FALSE;
	//�I�[�f�B�I������
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	//�}�X�^�[�{�C�X����
	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

}

void Sound::Finalize()
{
	xAudio2_.Reset();

	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();


	//�o�b�t�@�̃����������
	for (; it != soundDatas_.end(); ++it) {
		Unload(&it->second);
	}

	soundDatas_.clear();
	//SoundUnload(&soundData1);
	//SoundUnload(&soundData2);
}

void Sound::LoadWave(const std::string& filename)
{
	HRESULT result;

	if (soundDatas_.find(filename) != soundDatas_.end()) {
		return;
	}

	std::string fullpath = directoryPath_ + filename;

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
	if (strncmp(riff.chunk_.id_, "RIFF", 4) != 0) {
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type_, "WAVE", 4) != 0) {
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
	assert(format.chunk_.size_ <= sizeof(format.fmt_));
	file.read((char*)&format.fmt_, format.chunk_.size_);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id_, "JUNK", 4) == 0) {
		//�ǂݍ��݈ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size_, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id_, "data", 4) != 0) {
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size_];
	file.read(pBuffer, data.size_);

	//�t�@�C���N���[�Y
	file.close();

	//�ǂݍ��񂾉����f�[�^��retrun
	SoundData soundData = {};

	soundData.wfex_ = format.fmt_;
	soundData.pBuffer_ = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize_ = data.size_;

	soundDatas_.insert(std::make_pair(filename, soundData));
}

void Sound::Unload(SoundData* soundData)
{
	soundData->pBuffer_ = 0;
	soundData->bufferSize_ = 0;
	soundData->wfex_ = {};
}

void Sound::PlayWave(const std::string& filename)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	assert(it != soundDatas_.end());

	SoundData& soundData = it->second;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice* pSoundVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSoundVoice, &soundData.wfex_);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer_;
	buf.AudioBytes = soundData.bufferSize_;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	soundData.pSoundVoice_ = pSoundVoice;

	//�g�`�f�[�^�̍Đ�
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
