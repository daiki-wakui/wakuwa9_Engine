#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>
#include <cstdint>

struct Node
{
	//���O
	std::string name;
	float padding[2];

	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };

	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };

	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };

	//���[�J���ό`�s��
	DirectX::XMMATRIX transform = {};

	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform = {};

	//�e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
	

private: //�G�C���A�X

	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;


public: //�T�u�N���X

	//�{�[���C���f�b�N�X�̍ő吔
	static const int32_t sMAX_BONE_INDICES = 4;


	//���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;	//xyz���W
		DirectX::XMFLOAT3 normal;	//�@���x�N�g��
		DirectX::XMFLOAT2 uv;	//uv���W
		UINT boneIndex[sMAX_BONE_INDICES];	//�{�[�� �ԍ�
		float boneWeight[sMAX_BONE_INDICES];	//�{�[���@�d��
	};

	//�{�[���\����
	struct Bone
	{
		//���O
		std::string name;

		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;

		//�N���X�^�[
		FbxCluster* fbxCluster;

		//�R���X�g���N�^
		Bone(const std::string& name) {
			this->name = name;
		}
	};

public:

	//�t�����h�N���X
	friend class FbxLoder;

	//���f����
	std::string name_;

	//�m�[�h�z��
	std::vector<Node> nodes_;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;

	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices_;

	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices_;

	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient_ = { 1,1,1 };

	//�f�B�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse_ = { 1,1,1 };

	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata_ = {};

	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg_ = {};

	//�{�[���z��
	std::vector<Bone> bones_;

	//FBX�V�[��
	FbxScene* fbxScene_ = nullptr;

	
private: //�����o�ϐ�

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff_;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff_;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;


public: //�����o�֐�

	//getter
	std::vector<Bone>& GetBones() { return bones_; }

	//getter
	FbxScene* GetFbxScene() { return fbxScene_; }

	//�f�X�g���N�^
	~FbxModel();

	void CreateBuffers(ID3D12Device* device);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���f���̕ό`�s����
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	
};

