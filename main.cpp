#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include <memory>
#include <string>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPI�̐����N���X
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* win = new WindowsApp();

	//keyborad�N���X�̐���
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectX�̊�Ր����N���X
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//windowsAPI������
	win->Initalize();
	windows.reset(win);
	MSG msg{};

	//DirectX������
	dxBasis->Initialize(win);
	DirectX.reset(dxBasis);

	//keyborad������
	input_->Initialize(win->GetHInstancee(), win->GetHwnd());
	keyboard.reset(input_);

#pragma region  �`�揉��������
//
//#pragma region  ���_�f�[�^
//
//	// ���_�f�[�^
//	XMFLOAT3 vertices[] = {
//	{ -0.5f, -0.5f, 0.0f }, // ����
//	{ -0.5f, +0.5f, 0.0f }, // ����
//	{ +0.5f, -0.5f, 0.0f }, // �E��
//	};
//	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
//	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
//
//#pragma endregion
//
//#pragma region  ���_�o�b�t�@�̐ݒ�
//	// ���_�o�b�t�@�̐ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
//	// ���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC resDesc{};
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//#pragma endregion
//
//#pragma region  ���_�o�b�t�@�̐���
//
//	// ���_�o�b�t�@�̐���
//	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
//	result = device->CreateCommittedResource(
//		&heapProp, // �q�[�v�ݒ�
//		D3D12_HEAP_FLAG_NONE,
//		&resDesc, // ���\�[�X�ݒ�
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//#pragma endregion
//
//#pragma region  ���_�o�b�t�@�ւ̃f�[�^�]��
//
//	//GPU��̃o�b�t�@�ɑΉ��������z������
//	XMFLOAT3* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	//�S���_�ɑ΂���
//	for (int i = 0; i < _countof(vertices); i++) {
//		vertMap[i] = vertices[i];
//	}
//	//�q���������
//	vertBuff->Unmap(0, nullptr);
//
//#pragma endregion
//
//#pragma region  ���_�o�b�t�@�r���[�̐���
//
//	// ���_�o�b�t�@�r���[�̍쐬
//	D3D12_VERTEX_BUFFER_VIEW vbView{};
//	// GPU���z�A�h���X
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// ���_�o�b�t�@�̃T�C�Y
//	vbView.SizeInBytes = sizeVB;
//	// ���_1���̃f�[�^�T�C�Y
//	vbView.StrideInBytes = sizeof(XMFLOAT3);
//
//#pragma endregion
//
//#pragma region  ���_�V�F�[�_�[�t�@�C���̓ǂݍ���
//
//	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
//	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
//	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
//	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
//	result = D3DCompileFromFile(
//		L"BasicVShader.hlsl", // �V�F�[�_�t�@�C����
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
//		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
//		0,
//		&vsBlob, &errorBlob);
//
//	// �G���[�Ȃ�
//	if (FAILED(result)) {
//		// errorBlob����G���[���e��string�^�ɃR�s�[
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		// �G���[���e���o�̓E�B���h�E�ɕ\��
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//
//#pragma endregion
//
//#pragma region  �s�N�Z���V�F�[�_�t�@�C���̓ǂݍ���
//
//	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
//	result = D3DCompileFromFile(
//		L"BasicPShader.hlsl", // �V�F�[�_�t�@�C����
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
//		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
//		0,
//		&psBlob, &errorBlob);
//
//	// �G���[�Ȃ�
//	if (FAILED(result)) {
//		// errorBlob����G���[���e��string�^�ɃR�s�[
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		// �G���[���e���o�̓E�B���h�E�ɕ\��
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//
//#pragma endregion
//
//#pragma region  ���_���C�A�E�g
//
//	// ���_���C�A�E�g
//	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
//	{
//	"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
//	D3D12_APPEND_ALIGNED_ELEMENT,
//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//	}, // (1�s�ŏ������ق������₷��)
//	};
//
//#pragma endregion
//
//#pragma region  �O���t�B�b�N�X�p�C�v���C���ݒ�
//
//	// �O���t�B�b�N�X�p�C�v���C���ݒ�
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
//
//	// �V�F�[�_�[�̐ݒ�
//	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
//	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
//	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
//	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
//
//	// �T���v���}�X�N�̐ݒ�
//	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
//
//	// ���X�^���C�U�̐ݒ�
//	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
//	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
//	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
//
//	// �u�����h�X�e�[�g
//	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
//		= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
//
//	// ���_���C�A�E�g�̐ݒ�
//	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
//	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
//
//	// �}�`�̌`��ݒ�
//	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//	// ���̑��̐ݒ�
//	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
//	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
//	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
//
//	// ���[�g�V�O�l�`��
//	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
//	// ���[�g�V�O�l�`���̐ݒ�
//	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
//	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//	// ���[�g�V�O�l�`���̃V���A���C�Y
//	ID3DBlob* rootSigBlob = nullptr;
//	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
//		&rootSigBlob, &errorBlob);
//	assert(SUCCEEDED(result));
//	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
//		IID_PPV_ARGS(&rootSignature));
//	assert(SUCCEEDED(result));
//	rootSigBlob->Release();
//	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
//	pipelineDesc.pRootSignature = rootSignature.Get();
//
//	//�p�C�v���C���X�e�[�g�̐���
//	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
//	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
//	assert(SUCCEEDED(result));
//
//#pragma endregion
//
#pragma endregion

	//�Q�[�����[�v
	while (true) {
		//�~�{�^���ŏI�����b�Z�[�W��������
		if (win->gameloopExit(msg) == true) {
			break;	//�Q�[�����[�v�I��
		}

		//keyborad�X�V����
		input_->Update();

#pragma region DirectX���t���[������

		//�`��O����
		dxBasis->PreDraw();

#pragma region  �p�C�v���C���X�e�[�g�ݒ�
//
//		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
//		commandList->SetPipelineState(pipelineState.Get());
//		commandList->SetGraphicsRootSignature(rootSignature.Get());
//
//#pragma endregion
//
//#pragma region  �v���j�e�B�u�`��
//
//		// �v���~�e�B�u�`��̐ݒ�R�}���h
//		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
//
//#pragma endregion
//
//#pragma region  ���_�o�b�t�@�r���[�̐ݒ�
//
//		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
//		commandList->IASetVertexBuffers(0, 1, &vbView);
//
//#pragma endregion
//
//#pragma region  �`��R�}���h
//
//		// �`��R�}���h
//		commandList->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
//
//#pragma endregion
//
#pragma endregion

		//�`��㏈��
		dxBasis->PostDraw();

#pragma endregion

	}

	//�E�B���h�E�N���X��o�^����
	win->Release();

	return 0;
}