#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

class ParticleManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X

	//���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos;	//xyz���W
		float scale;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
		XMMATRIX matBillboard;	//�r���{�[�h�s��
	};

private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	static const int vertexCount = 1024;		// ���_��

public: // �ÓI�����o�֐�
	
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	static void PostDraw();

	static const XMFLOAT3& GetEye() { return eye; }
	static const XMFLOAT3& GetTarget() { return target; }

	static void SetEye(XMFLOAT3 eye);
	static void SetTarget(XMFLOAT3 target);

	
	static void CameraMoveVector(XMFLOAT3 move);

	//�x�N�g���ɂ�鎋�_�ړ�
	static void CameraMoveEyeVector(XMFLOAT3 move);

	static void SetCamera(XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) {
		ParticleManager::eye = eye,
		ParticleManager::target = target,
		ParticleManager::up = up;
	}

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
//	static ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// ���_�f�[�^�z��
	static VertexPos vertices[vertexCount];

	//�r���{�[�h�s��
	static XMMATRIX matBillborad;
	//Y���r���{�[�h�s��
	static XMMATRIX matBillboradY;

private:// �ÓI�����o�֐�

	//�f�X�N���v�^�q�[�v�̏�����
	static void InitializeDescriptorHeap();

	//�J����������
	static void InitializeCamera(int window_width, int window_height);

	//�O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();

	//�e�N�X�`���ǂݍ���
	static void LoadTexture();

	//���f���쐬
	static void CreateModel();

	//�r���[�s����X�V
	static void UpdateViewMatrix();

public: // �����o�֐�

	bool Initialize();
	void Update();
	void Draw();

	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
		float start_scale, float end_scale);

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };


	struct Particle
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;

		XMFLOAT3 position = {};
		XMFLOAT3 velocity = {};
		XMFLOAT3 accel = {};
		int frame = 0;
		int num_frame = 0;
		float scale = 1.0f;
		float s_scale = 1.0f;
		float e_scale = 0.0f;
	};

	std::forward_list<Particle> particles;

};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);

