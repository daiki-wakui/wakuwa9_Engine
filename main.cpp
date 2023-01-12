#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <xaudio2.h>
#pragma comment (lib,"xaudio2.lib")
#include <fstream>
#include <wrl.h>

#include <memory>
#include <list>

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

SoundData SoundLoadWave(const char* filename) {
	HRESULT result;

	//�t�@�C���I�[�v��
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);
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

	return soundData;
}

//�����f�[�^���
void SoundUnload(SoundData* soundData) {
	//�o�b�t�@�̃����������
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

//�����Đ�
void SoundPlayWave(IXAudio2* xAudio2, SoundData& soundData) {
	HRESULT result;

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

	//
}

void SoundStopWAVE(IXAudio2* xAudio2, const SoundData& soundData) {
	soundData.pSoundVoice_->Stop();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPI�̐����N���X
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyborad�N���X�̐���
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectX�̊�Ր����N���X
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//windowsAPI������
	winApp->Initalize();
	windows.reset(winApp);
	MSG msg{};

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	//DirectX������
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	HRESULT result = S_FALSE;
	//�I�[�f�B�I������
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//keyborad������
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	

#pragma region  �`�揉��������
	//HRESULT result;

#pragma region  �摜�C���[�W�f�[�^

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/test.png",
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

#pragma endregion

#pragma region  �~�j�}�b�v�̐���

	ScratchImage mipChain{};
	//�~�j�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,0,mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

#pragma endregion

#pragma region  �t�H�[�}�b�g������������
	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

#pragma endregion

#pragma region  �e�N�X�`���o�b�t�@�ݒ�

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;


#pragma endregion

#pragma region  �e�N�X�`���o�b�t�@�̐���

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

#pragma endregion

#pragma region  �e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	//�f�[�^�]��
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0);
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

#pragma endregion

#pragma region  �e�N�X�`���T���v���[�̐ݒ�
	//�e�N�X�`������ǂ̂悤�ɐF�����o�����̐ݒ�
	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;	//�~�j�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;	//�~�j�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

#pragma endregion

#pragma region  �f�X�N���v�^�q�[�v����

	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
	result = dxBasis->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRV�q�[�v�̐擪�A�h���X���擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region  �V�F�[�_���\�[�X�r���[�̐���

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
	dxBasis->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

#pragma endregion

#pragma region  �f�X�N���v�^�����W�̐ݒ�
	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion


#pragma region  �萔�o�b�t�@

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial{
		XMFLOAT4 color;	//�F(RGBA)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct CounstBufferDataTransform{
		XMMATRIX mat;	//3D�ϊ��s��
	};

	CounstBufferDataTransform* constMapTransform = nullptr;

#pragma region  �萔�o�b�t�@�̐ݒ�

	//�q�[�v�ݒ�(color)
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	
	//�q�[�v�ݒ�(mat)
	D3D12_HEAP_PROPERTIES cbMatHeapProp{};
	cbMatHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbMatResourceDesc{};
	cbMatResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMatResourceDesc.Width = (sizeof(CounstBufferDataTransform) + 0xff) & ~0xff;
	cbMatResourceDesc.Height = 1;
	cbMatResourceDesc.DepthOrArraySize = 1;
	cbMatResourceDesc.MipLevels = 1;
	cbMatResourceDesc.SampleDesc.Count = 1;
	cbMatResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	

#pragma endregion

#pragma region  �萔�o�b�t�@�̐���

	//color
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//�萔�o�b�t�@�̐���
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//mat
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//�萔�o�b�t�@�̐���
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&cbMatHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbMatResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));


#pragma endregion

#pragma region	�萔�o�b�t�@�̃}�b�s���O

	//�萔�o�b�t�@�̃}�b�s���O(color)
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O(mat)
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region	�萔�o�b�t�@�ւ̃f�[�^�]��

	//�l����������Ŏ����]��
	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);	//�F�ύX


#pragma region	���s���e�ϊ�
	//���s���e�ϊ�
	constMapTransform->mat = XMMatrixIdentity();

	XMMATRIX matProjection=
		XMMatrixOrthographicOffCenterLH(
			0.0f, winApp->GetWindowWidth(),
			winApp->GetWindowHeight(), 0.0f,
			0.0f, 1.0f);

	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	XMMATRIX matScale;
	matScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld *= matScale;

	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));	//Z 0�x��]
	matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));	//X 0�x��]
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));	//Y 0�x��]
	matWorld *= matRot;

	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	matWorld *= matTrans;

	constMapTransform->mat = matWorld * matProjection;

	XMFLOAT2 pos = { 0,0 };

#pragma endregion

#pragma endregion

#pragma endregion

#pragma region  ���_�f�[�^

	//���_�f�[�^�\����
	struct Vertex{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	// ���_�f�[�^
	Vertex vertices[] = {
		{{   0.0f, 100.0f, 0.0f },{ 0.0f , 1.0f }}, // ����
		{{   0.0f,   0.0f, 0.0f },{ 0.0f , 0.0f }}, // ����
		{{ 100.0f, 100.0f, 0.0f },{ 1.0f , 1.0f }}, // ����
		{{ 100.0f,   0.0f, 0.0f },{ 1.0f , 0.0f }}, // ����
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

#pragma endregion

#pragma region  �C���f�b�N�X�f�[�^

	//�C���f�b�N�X�f�[�^
	uint16_t indices[] = {
		0 , 1 , 2,	//�O�p�`1��
		1 , 2 , 3,	//�O�p�`2��
	};

#pragma endregion

#pragma region  ���_�o�b�t�@�̐ݒ�
	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region  ���_�o�b�t�@�̐���

	// ���_�o�b�t�@�̐���
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region  ���_�o�b�t�@�ւ̃f�[�^�]��

	//GPU��̃o�b�t�@�ɑΉ��������z������
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
	}
	//�q���������
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region  ���_�o�b�t�@�r���[�̐���

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion

#pragma region  �C���f�b�N�X�o�b�t�@�̐ݒ�

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region  �C���f�b�N�X�o�b�t�@�̐���

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
	result = dxBasis->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

#pragma endregion

#pragma region  �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region  �C���f�b�N�X�o�b�t�@�r���[�̐���

	//�C���f�b�N�X�o�b�t�@�r���[�̐���
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

#pragma region  ���_�V�F�[�_�[�t�@�C���̓ǂݍ���

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVShader.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion

#pragma region  �s�N�Z���V�F�[�_�t�@�C���̓ǂݍ���

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPShader.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion

#pragma region  ���_���C�A�E�g

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	//uv���W
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

#pragma endregion

#pragma region  �O���t�B�b�N�X�p�C�v���C���ݒ�

#pragma region  �O���t�B�b�N�X�p�C�v���C���ɕK�v�Ȑݒ�

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma endregion

#pragma region  �u�����h�ݒ�

	// �u�����h�X�e�[�g
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��

	//���ʐݒ�(�A���t�@�l)
	blenddesc.BlendEnable = true;	//�u�����h��L��
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	//���Z����
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/

	//���Z����
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/

	//�F���]
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blenddesc.DestBlend = D3D12_BLEND_ZERO;*/

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

#pragma endregion

#pragma region  ���_���C�A�E�g�E���̑��`��ݒ�

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

#pragma endregion

#pragma region  ���[�g�p�����[�^

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3];
	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;	//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;		//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;	//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;		//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
#pragma endregion

#pragma region  ���[�g�V�O�l�`��

	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxBasis->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

#pragma endregion
	
#pragma region  �p�C�v���C���X�e�[�g����

	//�p�C�v���C���X�e�[�g�̐���
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	result = dxBasis->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma endregion

#pragma endregion

	//��
	SoundData soundData1 = SoundLoadWave("Resources/Sound/PerituneMaterial.wav");
	SoundData soundData2 = SoundLoadWave("Resources/Sound/Hit.wav");
	SoundData soundData3 = SoundLoadWave("Resources/Sound/Electric Wild.wav");
	SoundData soundData4 = SoundLoadWave("Resources/Sound/Shot.wav");
	SoundData soundData5 = SoundLoadWave("Resources/Sound/damage.wav");
	SoundData soundData6 = SoundLoadWave("Resources/Sound/dash.wav");

	int PlayBGM = 0;
	int ChangeBGM = 0;

	//OBJ���烂�f����ǂݍ���
	Model* model = Model::LoadFromObj("boss");
	Model* model2 = Model::LoadFromObj("world");

	Model* playerModel = Model::LoadFromObj("Cube");
	//Model* playerBulletModel = Model::LoadFromObj("playerBullet");
	Model* floorModel = Model::LoadFromObj("Floor");

	Model* fieldBlock = Model::LoadFromObj("bobj");
	Model* fieldBlock2 = Model::LoadFromObj("aobj");

	Model* playerModel_ = Model::LoadFromObj("player");
	Model* podModel_ = Model::LoadFromObj("pad");
	Model* enemyModel_ = Model::LoadFromObj("enemy");

	Model* UIModel[13];
	UIModel[0] = Model::LoadFromObj("gameover");
	UIModel[1] = Model::LoadFromObj("kaihi");
	UIModel[2] = Model::LoadFromObj("move");

	UIModel[3] = Model::LoadFromObj("rot");
	UIModel[4] = Model::LoadFromObj("spaceStart");
	UIModel[5] = Model::LoadFromObj("spaceTitle");
	UIModel[6] = Model::LoadFromObj("wave13");
	UIModel[7] = Model::LoadFromObj("wave23");
	UIModel[8] = Model::LoadFromObj("wave33");
	UIModel[9] = Model::LoadFromObj("retry");
	UIModel[10] = Model::LoadFromObj("auto");
	UIModel[11] = Model::LoadFromObj("buster");
	UIModel[12] = Model::LoadFromObj("gameclear");

#pragma region  �I�u�W�F�N�g����

	//UI
	Object3D* UIobj[13];

	for (int i = 0; i < 13; i++) {
		UIobj[i] = Object3D::Create(UIModel[i], { 3,3,3 });
	}

	//�^�C�g����UI
	UIobj[10]->SetPosition({ -40,20,50 });
	UIobj[11]->SetPosition({ 30,10,25 });

	//3D�I�u�W�F�N�g����
	Object3D* object3d3 = Object3D::Create(model2, { (500.0f),(500.0f),(500.0f) });

	Object3D* enemyObject = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject2 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject3 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });

	Object3D* enemyObject4 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject5 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject6 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject7 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });


	Object3D* playerObject = Object3D::Create(playerModel_, { (2.0f),(2.0f),(2.0f) });
	Object3D* podObject = Object3D::Create(podModel_, { (2),(2),(2) });

	Object3D* floorObject = Object3D::Create(floorModel, { (25.0f),(25.0f),(25.0f) });
	Object3D* fieldblock = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock2 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock3 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock4 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock5 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	Object3D* fieldblock6 = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock7 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock8 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock9 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock10 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	Object3D* fieldblock11 = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock12 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock13 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock14 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock15 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	playerObject->SetPosition({ 0,0,-20 });
	floorObject->SetPosition({ 0,-10,0 });

	fieldblock->SetPosition({ -40,40,300 });
	fieldblock2->SetPosition({ 150,20,300 });
	fieldblock3->SetPosition({ -150,80,300 });
	fieldblock4->SetPosition({ 30,100,300 });
	fieldblock5->SetPosition({ -100,200,300 });

	fieldblock6->SetPosition({ 300,40,120 });
	fieldblock7->SetPosition({ 300,20,0 });
	fieldblock8->SetPosition({ 300,80,-50 });
	fieldblock9->SetPosition({ 270,100,280 });
	fieldblock10->SetPosition({ 300,150,200 });

	fieldblock11->SetPosition({ -300,40,200 });
	fieldblock12->SetPosition({ -300,120,280 });
	fieldblock13->SetPosition({ -300,80,-20 });
	fieldblock14->SetPosition({ -250,20,-100 });
	fieldblock15->SetPosition({ -350,150,150 });


	fieldblock4->SetRotation({ 0,90,0 });
	fieldblock5->SetRotation({ 0,-90,0 });
	fieldblock9->SetRotation({ 0,90,0 });
	fieldblock14->SetRotation({ 0,-90,0 });
#pragma endregion

	Object3D::CameraMoveVector({ 0.0f,20.0f,-30.0f });

	float angle = 0.0f;

	Player* player = new Player;
	player->Initialize(playerModel_, playerObject, input_, podObject);

	int isPlayerDamege = 0;
	int invincibleTime = 0;
	int invincible = 0;

	int wave = 0;
	std::list<std::unique_ptr<Enemy>> enemys_;

	int isPop = 0;
	int scene = 0;
	int knockDownNum = 0;
	int popCoolTime = 0;
	XMFLOAT3 domeRot={};

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (winApp->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}

		//-----��������X�V����-----//
		//�V�[���؂�ւ�
		//�^�C�g��
		if (scene == 0) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 1;
				player->HP = 5;
				player->isDead = false;
				playerObject->SetPosition({ 0,0,-20 });
				playerObject->SetRotation({ 0,0,0 });
				Object3D::SetCamera();
				enemys_.clear();
				player->clear();
				wave = 0;
				isPop = 0;
				knockDownNum = 0;
				popCoolTime = 0;
				isPlayerDamege = 0;
				invincibleTime = 0;
				invincible = 0;
			}
		}
		//�Q�[���V�[��
		else if (scene == 1) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 2;
			}
		}
		//�Q�[���I�[�o�[
		else if (scene == 2) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 1;
				player->HP = 5;
				player->isDead = false;
				playerObject->SetPosition({ 0,0,-20 });
				playerObject->SetRotation({ 0,0,0 });
				Object3D::SetCamera();
				enemys_.clear();
				player->clear();
				wave = 0;
				isPop = 0;
				knockDownNum = 0;
				popCoolTime = 0;
				isPlayerDamege = 0;
				invincibleTime = 0;
				invincible = 0;
			}
		}
		//�Q�[���N���A
		else if (scene == 3) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 0;
			}
		}

		/*if (PlayBGM == 0 && ChangeBGM == 0) {
			SoundPlayWave(xAudio2.Get(), soundData1);
			PlayBGM = 1;
		}
		if (PlayBGM == 0 && ChangeBGM == 1) {
			SoundPlayWave(xAudio2.Get(), soundData3);
			PlayBGM = 1;
		}

		if (input_->keyInstantPush(DIK_G)) {
			if (ChangeBGM == 0) {
				SoundStopWAVE(xAudio2.Get(), soundData1);
			}
			PlayBGM = 0;
			ChangeBGM = 1;
		}*/

		//keyborad�X�V����
		input_->Update();

#pragma region  �I�u�W�F�N�g�X�V����

		/*if (isPlayerDamege == 1) {
			domeRot = object3d3->GetRotation();

			domeRot.y += 50;

			object3d3->SetRotation(domeRot);
		}*/

		if (scene == 0) {
			for (int i = 0; i < 13; i++) {
				UIobj[i]->Update();
			}
		}

		object3d3->Update();

		if (scene == 1) {

			if (input_->keyInstantPush(DIK_K)) {
				player->OnCollision();
			}

			if (player->IsDead() == true) {
				scene = 2;
			}

#pragma region  �E�F�[�u�Ǘ�

			//�`���[�g���A��
			if (wave == 0) {
				if (isPop == 0) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy3 = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject, { -70,0,100 }, player);
					newEnemy2->Initialize(enemyObject2, { 0,0,100 }, player);
					newEnemy3->Initialize(enemyObject3, { 70,0,100 }, player);

					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					enemys_.push_back(std::move(newEnemy2));
					enemys_.push_back(std::move(newEnemy3));
					isPop++;
				}

				if (knockDownNum == 3) {
					wave++;
					knockDownNum = 0;
					isPop = 0;
				}
			}

			//�E�F�[�u1
			if (wave == 1) {
				popCoolTime++;

				if (popCoolTime >= 300) {
					if (isPop == 0) {
						std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
						std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
						std::unique_ptr<Enemy> newEnemy3 = std::make_unique<Enemy>();

						newEnemy->Initialize(enemyObject, { -70,0,50 }, player);
						newEnemy2->Initialize(enemyObject2, { 0,0,100 }, player);
						newEnemy3->Initialize(enemyObject3, { 70,0,50 }, player);

						//�G��o�^����
						enemys_.push_back(std::move(newEnemy));
						enemys_.push_back(std::move(newEnemy2));
						enemys_.push_back(std::move(newEnemy3));
						popCoolTime = 0;
						isPop = 1;
					}
				}

				if (popCoolTime >= 100) {
					if (isPop == 1) {
						std::unique_ptr<Enemy> newEnemy4 = std::make_unique<Enemy>();
						std::unique_ptr<Enemy> newEnemy5 = std::make_unique<Enemy>();
						std::unique_ptr<Enemy> newEnemy6 = std::make_unique<Enemy>();

						newEnemy4->Initialize(enemyObject4, { -90,0,100 }, player);
						newEnemy5->Initialize(enemyObject5, { 0,0,50 }, player);
						newEnemy6->Initialize(enemyObject6, { 90,0,100 }, player);

						//�G��o�^����
						enemys_.push_back(std::move(newEnemy4));
						enemys_.push_back(std::move(newEnemy5));
						enemys_.push_back(std::move(newEnemy6));
						isPop++;
					}
				}

				//�S���|������
				if (knockDownNum == 6) {
					wave++;
					knockDownNum = 0;
					popCoolTime = 0;
					isPop = 0;
				}
			}

			//�E�F�[�u2
			if (wave == 2) {
				popCoolTime++;

				if (popCoolTime >= 300 && isPop == 0) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy3 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy4 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy5 = std::make_unique<Enemy>();
					std::unique_ptr<Enemy> newEnemy6 = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject, { -70,0,20 }, player);
					newEnemy2->Initialize(enemyObject2, { -50,0,60 }, player);
					newEnemy3->Initialize(enemyObject3, { -20,0,100 }, player);
					newEnemy4->Initialize(enemyObject4, { 20,0,100 }, player);
					newEnemy5->Initialize(enemyObject5, { 50,0,60 }, player);
					newEnemy6->Initialize(enemyObject6, { 70,0,20 }, player);

					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					enemys_.push_back(std::move(newEnemy2));
					enemys_.push_back(std::move(newEnemy3));
					enemys_.push_back(std::move(newEnemy4));
					enemys_.push_back(std::move(newEnemy5));
					enemys_.push_back(std::move(newEnemy6));

					isPop++;
				}

				//�S���|������
				if (knockDownNum == 6) {
					wave++;
					knockDownNum = 0;
					popCoolTime = 0;
					isPop = 0;
				}
			}

			//�E�F�[�u3
			if (wave == 3) {
				popCoolTime++;

				if (popCoolTime >= 300 && isPop == 0) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject, { -70,0,0 }, player);
					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 1) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject2, { 70,0,30 }, player);
					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 2) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject3, { -50,0,60 }, player);
					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 3) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject4, { 50,0,90 }, player);
					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 4) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject5, { -20,0,120 }, player);
					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 50 && isPop == 5) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject6, { 20,0,120 }, player);
					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (popCoolTime >= 300 && isPop == 6 && knockDownNum == 6) {
					std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

					newEnemy->Initialize(enemyObject7, { 0,0,100 }, player,15);
					//�G��o�^����
					enemys_.push_back(std::move(newEnemy));
					popCoolTime = 0;
					isPop++;
				}

				if (knockDownNum == 7) {
					scene = 3;
				}
			}

#pragma endregion

			floorObject->Update();

			fieldblock->Update();
			fieldblock2->Update();
			fieldblock3->Update();
			fieldblock4->Update();
			fieldblock5->Update();
			fieldblock6->Update();
			fieldblock7->Update();
			fieldblock8->Update();
			fieldblock9->Update();
			fieldblock10->Update();
			fieldblock11->Update();
			fieldblock12->Update();
			fieldblock13->Update();
			fieldblock14->Update();
			fieldblock15->Update();

			player->Update();

			//����SE
			if (player->GetCoolTime() == 0) {
				SoundPlayWave(xAudio2.Get(), soundData4);
			}

			if (player->GetIsStep() == true && input_->keyInstantPush(DIK_W)) {
				SoundPlayWave(xAudio2.Get(), soundData6);
			}

			//
			//���G����
			if (player->GetIsStep() == true) {
				invincible = true;
			}
			else {
				invincible = false;
			}

			if (isPlayerDamege == 1) {
				invincibleTime++;

				if (invincibleTime >= 100) {
					isPlayerDamege = 0;
					invincibleTime = 0;
				}
			}

			//enemy�̎��S�t���O
			enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
				return enemy->IsDead();
			});

			//�G�̓���
			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Update();
			}
		}
		
		if (scene == 2) {
			floorObject->Update();
			fieldblock->Update();
			fieldblock2->Update();
			fieldblock3->Update();
			fieldblock4->Update();
			fieldblock5->Update();
			fieldblock6->Update();
			fieldblock7->Update();
			fieldblock8->Update();
			fieldblock9->Update();
			fieldblock10->Update();
			fieldblock11->Update();
			fieldblock12->Update();
			fieldblock13->Update();
			fieldblock14->Update();
			fieldblock15->Update();

			//�G�̓���
			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Update();
			}
		}
		

#pragma endregion

		XMMATRIX matTrans;
		matTrans = XMMatrixTranslation(pos.x, pos.y, 0.0f);
		matWorld *= matTrans;

		constMapTransform->mat = matWorld * matProjection;

#pragma region  �����蔻��

		if (scene == 1) {
			XMFLOAT3 posA, posB;

			//���e���X�g�̎擾
			const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

			posA = player->GetWorldPos();

			//���@�ƓG�̒e�̓����蔻��
			for (const std::unique_ptr<Enemy>& enemy : enemys_) {
				for (const std::unique_ptr<EnemyBullet>& bullet : enemy->GetBullets()) {
					//�G�e�̍��W
					posB = bullet->GetWorldPos();

					//A��B�̋���
					float r1 = 7.0f;	//�G�̃X�P�[��
					float r2 = 1.0f;	//�e�̃X�P�[��
					float r = r1 + r2;

					XMFLOAT3 dis;
					dis.x = posB.x - posA.x;
					dis.y = posB.y - posA.y;
					dis.z = posB.z - posA.z;

					if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
						
						if (isPlayerDamege == 0 && invincible == 0) {
							bullet->isDead_ = true;
							isPlayerDamege = 1;
							player->OnCollision();
							SoundPlayWave(xAudio2.Get(), soundData5);
						}
					}
				}
			}

			for (const std::unique_ptr<Enemy>& enemy : enemys_) {
				posA = enemy->GetWorldPos();

				//���@�̒e�ƓG�̓����蔻��
				for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
					//���e�̍��W
					posB = bullet->GetWorldPos();

					//A��B�̋���
					float r1 = 7.0f;	//�G�̃X�P�[��
					float r2 = 1.0f;	//�e�̃X�P�[��
					float r = r1 + r2;

					XMFLOAT3 dis;
					dis.x = posB.x - posA.x;
					dis.y = posB.y - posA.y;
					dis.z = posB.z - posA.z;


					if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
						bullet->isDead_ = true;
						enemy->OnCollision();

						if (enemy->IsDead() == true) {
							knockDownNum++;
						}
						SoundPlayWave(xAudio2.Get(), soundData2);
					}
				}
			}
		}
		

#pragma endregion

		//��������`�揈��

		if (scene == 0) {
			// �`��O����
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

			//�V��
			object3d3->Draw();

			//UI->Draw();
			//UI2->Draw();

			//clearUI->Draw();
			//UIobj[0]->Draw();
			//UIobj[1]->Draw();
			//UIobj[2]->Draw();

			//UIobj[3]->Draw();
			//UIobj[4]->Draw();

			//UIobj[5]->Draw();
			//UIobj[6]->Draw();
			//UIobj[7]->Draw();
			//UIobj[8]->Draw();
			UIobj[9]->Draw();
			for (int i = 0; i < 11; i++) {
				
			}

			Object3D::PostDraw();

			//�`��㏈��
			dxBasis->PostDraw();
		}
		else if (scene == 1) {
#pragma region DirectX���t���[������

			// �`��O����
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

#pragma region  3D���f���`�揈��
			object3d3->Draw();

			floorObject->Draw();

			fieldblock->Draw();
			fieldblock2->Draw();
			fieldblock3->Draw();
			fieldblock4->Draw();
			fieldblock5->Draw();
			fieldblock6->Draw();
			fieldblock7->Draw();
			fieldblock8->Draw();
			fieldblock9->Draw();
			fieldblock10->Draw();
			fieldblock11->Draw();
			fieldblock12->Draw();
			fieldblock13->Draw();
			fieldblock14->Draw();
			fieldblock15->Draw();

			//playerObject->Draw();
			player->Draw();

			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Draw();
			}

			/*for (int i = 0; i < 3; i++) {
				enemy[i]->Draw();
			}*/
#pragma endregion

			Object3D::PostDraw();

			// �p�C�v���C���X�e�[�g�ݒ�
			dxBasis->GetCommandList()->SetPipelineState(pipelineState.Get());

			// ���[�g�V�O�l�`���̐ݒ�R�}���h
			dxBasis->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

			// �v���~�e�B�u�`��̐ݒ�R�}���h
			dxBasis->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

			// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
			dxBasis->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

			//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
			ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
			dxBasis->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
			//SRV�q�[�v�̐擪�A�h���X���擾
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
			//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
			dxBasis->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
			dxBasis->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

			// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
			dxBasis->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

			// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
			dxBasis->GetCommandList()->IASetIndexBuffer(&ibView);

			// �`��R�}���h
			dxBasis->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��

			//�`��㏈��
			dxBasis->PostDraw();

#pragma endregion
		}
		else if (scene == 2) {
			// �`��O����
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

			//�V��
			object3d3->Draw();

			floorObject->Draw();

			fieldblock->Draw();
			fieldblock2->Draw();
			fieldblock3->Draw();
			fieldblock4->Draw();
			fieldblock5->Draw();
			fieldblock6->Draw();
			fieldblock7->Draw();
			fieldblock8->Draw();
			fieldblock9->Draw();
			fieldblock10->Draw();
			fieldblock11->Draw();
			fieldblock12->Draw();
			fieldblock13->Draw();
			fieldblock14->Draw();
			fieldblock15->Draw();

			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Draw();
			}

			Object3D::PostDraw();

			//�`��㏈��
			dxBasis->PostDraw();
		}
		else if (scene == 3) {
		// �`��O����
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		//�V��
		object3d3->Draw();

		Object3D::PostDraw();

		//�`��㏈��
		dxBasis->PostDraw();
		}
	}

	delete model;
	delete model2;
	delete playerModel;
	delete floorModel;
	delete fieldBlock;
	delete fieldBlock2;

	delete enemyObject;
	delete enemyObject2;
	delete enemyObject3;

	delete floorObject;
	delete playerObject;
	delete object3d3;
	delete player;
	delete fieldblock;
	delete fieldblock2;
	delete fieldblock3;
	delete fieldblock4;
	delete fieldblock5;
	delete fieldblock6;
	delete fieldblock7;
	delete fieldblock8;
	delete fieldblock9;
	delete fieldblock10;
	delete fieldblock11;
	delete fieldblock12;
	delete fieldblock13;
	delete fieldblock14;
	delete fieldblock15;

	xAudio2.Reset();
	SoundUnload(&soundData1);
	SoundUnload(&soundData2);
	SoundUnload(&soundData3);
	SoundUnload(&soundData4);

	//�E�B���h�E�N���X��o�^����
	winApp->Release();

	return 0;
}