#include "PostEffect.h"
#include <d3dx12.h>
#include <cassert>

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

const float PostEffect::sCLEAR_COLOR[4] = { 0.25f,0.5f,0.1f,0.0f };//�΂��ۂ��F

void PostEffect::Initialize()
{
	HRESULT result;

	//Sprite::Initialize(spBasis_, winApp_);

	VertexData();
	IndexData();


	//�e�N�X�`���o�b�t�@�ݒ�
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//�e�N�X�`�����\�[�X�ݒ�
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		winApp_->GetWindowWidth(),
		(UINT)winApp_->GetWindowHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_CLEAR_VALUE clearValue;

	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	for (int32_t i = 0; i < 4; i++) {
		clearValue.Color[i] = sCLEAR_COLOR[i];
	}

	/*clearValue.Color[0] = sCLEAR_COLOR[0];
	clearValue.Color[1] = sCLEAR_COLOR[1];
	clearValue.Color[2] = sCLEAR_COLOR[2];
	clearValue.Color[3] = sCLEAR_COLOR[3];*/

	//�e�N�X�`���o�b�t�@�̐���
	for (int32_t i = 0; i < 2; i++) {
		result = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(&texBuff_[i]));

		assert(SUCCEEDED(result));


		//�C���[�W�f�[�^�]��
		const UINT pixelCount = winApp_->GetWindowWidth() * winApp_->GetWindowHeight();

		const UINT rowPitch = sizeof(UINT) * winApp_->GetWindowHeight();

		const UINT deptPitch = rowPitch * winApp_->GetWindowHeight();

		UINT* img = new UINT[pixelCount];

		for (UINT j = 0; j < pixelCount; j++) {
			img[j] = 0xff0000ff;
		}

		result = texBuff_[i]->WriteToSubresource(0, nullptr, img, rowPitch, deptPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	

	

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;

	//SRV�p�f�X�N���v�^�q�[�v����
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&srvDescHeapDesc,
		IID_PPV_ARGS(&descHeapSRV_));

	assert(SUCCEEDED(result));


	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	spBasis_->GetDxBasis()->GetDevice()->CreateShaderResourceView(
		texBuff_[0].Get(),
		&srvDesc,
		descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
	);

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	//RTV�p�f�X�N���v�^�q�[�v����
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&rtvDescHeapDesc,
		IID_PPV_ARGS(&descHeapRTV));

	assert(SUCCEEDED(result));


	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	

	//CD3DX12_CPU_DESCRIPTOR_HANDLE r;

	for (int32_t i = 0; i < 2; i++) {
		//�f�X�N���v�^�q�[�v��RTV����
		spBasis_->GetDxBasis()->GetDevice()->CreateRenderTargetView(texBuff_[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(),i,
				spBasis_->GetDxBasis()->GetDevice()->GetDescriptorHandleIncrementSize(
					D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
			
		);
	}




	//�[�x�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApp_->GetWindowWidth();
	depthResourceDesc.Height = winApp_->GetWindowHeight();
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//�f�v�X�X�e���V��

	// �[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//�[�x�l1.0f(�ő�l)
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//�[�x�o�b�t�@�̐���
	result = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_));

	//DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NumDescriptors = 1;

	//DRV�p�f�X�N���v�^�q�[�v����
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV_));

	assert(SUCCEEDED(result));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	spBasis_->GetDxBasis()->GetDevice()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart());

	Crate();

	//�p�C�v���C������
	CreateGraphicsPipelineState();
}

void PostEffect::VertexData()
{

	// ���_�f�[�^
	vertices_[LB] = { {   0.0f, (float)winApp_->GetWindowHeight()/2, 0.0f},{0.0f , 1.0f}};
	vertices_[LT] = { {   0.0f, 0.0f, 0.0f }, { 0.0f , 0.0f } };
	vertices_[RB] = { { (float)winApp_->GetWindowWidth()/2, (float)winApp_->GetWindowHeight()/2, 0.0f},{1.0f , 1.0f}};
	vertices_[RT] = { { (float)winApp_->GetWindowWidth()/2,   0.0f, 0.0f },{ 1.0f , 0.0f } };

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));

	vertexSize_ = _countof(vertices_);

	// ���_�o�b�t�@�̐ݒ�
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	// ���_�o�b�t�@�̐���

	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp_, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

#pragma region  ���_�o�b�t�@�ւ̃f�[�^�]��

	//GPU��̃o�b�t�@�ɑΉ��������z������
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	//�S���_�ɑ΂���
	for (int32_t i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i];
	}
	//�q���������
	vertBuff_->Unmap(0, nullptr);

#pragma endregion

#pragma region  ���_�o�b�t�@�r���[�̐���

	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion
}

void PostEffect::IndexData()
{
	//�C���f�b�N�X�f�[�^
	uint16_t indices[] = {
		0 , 1 , 2,	//�O�p�`1��
		1 , 2 , 3,	//�O�p�`2��
	};

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	indexSize_ = _countof(indices);

	//���\�[�X�ݒ�
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeIB;	//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	//�C���f�b�N�X�o�b�t�@�̐���
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp_,
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int32_t i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	//�}�b�s���O����
	indexBuff_->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̐���
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

void PostEffect::Crate()
{
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


	//color
	//�萔�o�b�t�@�̐���
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result_));

	//mat
	//�萔�o�b�t�@�̐���
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbMatHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbMatResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));

	//�萔�o�b�t�@�̃}�b�s���O(color)
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result_ = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	assert(SUCCEEDED(result_));

	//�萔�o�b�t�@�̃}�b�s���O(mat)
	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result_));

	//�l����������Ŏ����]��
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);	//�F�ύX

	//��������_�ɐݒ�
	matProjection_ =
		XMMatrixOrthographicOffCenterLH(
			0.0f, (float)winApp_->GetWindowWidth(),
			(float)winApp_->GetWindowHeight(), 0.0f,
			0.0f, 1.0f);

	matWorld_ = XMMatrixIdentity();

	matScale_ = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld_ *= matScale_;

	matRot_ = XMMatrixIdentity();
	matRot_ *= XMMatrixRotationZ(XMConvertToRadians(0.0f));	//Z 0�x��]
	matRot_ *= XMMatrixRotationX(XMConvertToRadians(0.0f));	//X 0�x��]
	matRot_ *= XMMatrixRotationY(XMConvertToRadians(0.0f));	//Y 0�x��]
	matWorld_ *= matRot_;

	matTrans_ = XMMatrixTranslation(1280 / 2 - 640 / 2, 720 / 2 - 360 / 2, 0.0f);
	matWorld_ *= matTrans_;

	constMapTransform_->mat = matWorld_ * matProjection_;

	XMFLOAT2 pos = { 0,0 };

}

void PostEffect::SetDirectX(SpriteBasis* spBasis, WindowsApp* winApp, KeyBoard* key)
{
	spBasis_ = spBasis;
	winApp_ = winApp;
	key_ = key;
}

PostEffect::PostEffect()
{

}

void PostEffect::CreateGraphicsPipelineState()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g


	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl", // �V�F�[�_�t�@�C����
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

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectPS.hlsl", // �V�F�[�_�t�@�C����
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

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//xy���W
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



	// �u�����h�X�e�[�g
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;


	//���ʐݒ�(�A���t�@�l)
	blenddesc.BlendEnable = true;	//�u�����h��L��
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//�f�X�N���v�^�����W�̐ݒ�
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
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


	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//�e�N�X�`������ǂ̂悤�ɐF�����o�����̐ݒ�
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;	//�~�j�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;	//�~�j�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ���[�g�V�O�l�`��
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

	result = spBasis_->GetDxBasis()->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	rootSigBlob->Release();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	result = spBasis_->GetDxBasis()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));


}

void PostEffect::Draw()
{

	if (key_->keyInstantPush(DIK_0)) {
		static int32_t tex = 0;

		tex = (tex + 1) % 2;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		spBasis_->GetDxBasis()->GetDevice()->CreateShaderResourceView(
			texBuff_[tex].Get(),
			&srvDesc,
			descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
		);
	}

	// �p�C�v���C���X�e�[�g�ݒ�
	spBasis_->GetDxBasis()->GetCommandList()->SetPipelineState(pipelineState.Get());

	// ���[�g�V�O�l�`���̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	//�v���j�e�B�u�`���ݒ�
	spBasis_->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV_.Get() };
	spBasis_->GetDxBasis()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRV�q�[�v�̐擪�A�h���X���擾
	/*srvGpuHandle_ = spBasis_->srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle_.ptr += spBasis_->GetincrementSize() * texNum;*/

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	spBasis_->GetDxBasis()->GetCommandList()->IASetIndexBuffer(&ibView_);

	// �`��R�}���h
	//dxBasis->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
	spBasis_->GetDxBasis()->GetCommandList()->DrawIndexedInstanced(indexSize_, 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	for (int32_t i = 0; i < 2; i++) {
		// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		barrierDesc_.Transition.pResource = texBuff_[i].Get(); // �o�b�N�o�b�t�@���w��
		barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // �\����Ԃ���
		barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
		cmdList->ResourceBarrier(1, &barrierDesc_);
	}
	

	/*CD3DX12_RESOURCE_BARRIER resouceBarrier;

	resouceBarrier.Transition(texBuff_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);*/

	//���\�[�X�o���A��ύX
	//cmdList->ResourceBarrier(1,&resouceBarrier);

	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];

	for (int32_t i = 0; i < 2; i++) {
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			spBasis_->GetDxBasis()->GetDevice()->GetDescriptorHandleIncrementSize(
				D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}


	//�[�x�X�e���V���r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV_->GetCPUDescriptorHandleForHeapStart();

	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(2, rtvHs, false, &dsvH);

	// �r���[�|�[�g�ݒ�R�}���h
	//D3D12_VIEWPORT viewport{};
	//viewport.Width = (FLOAT)winApp_->GetWindowWidth();
	//viewport.Height = (FLOAT)winApp_->GetWindowHeight();
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.MinDepth = 0.0f;
	//viewport.MaxDepth = 1.0f;

	CD3DX12_VIEWPORT viewports[2];

	// �V�U�[��`
	D3D12_RECT scissorRects[2] = {};

	for (int32_t i = 0; i < 2; i++) {
		viewports[i].TopLeftX = 0.0f;
		viewports[i].TopLeftY = 0.0f;
		viewports[i].Width = (float)winApp_->GetWindowWidth();
		viewports[i].Height = (float)winApp_->GetWindowHeight();
		viewports[i].MinDepth = 0.0f;
		viewports[i].MaxDepth = 1.0f;

		scissorRects[i].left = 0; // �؂蔲�����W��
		scissorRects[i].right = scissorRects[i].left + winApp_->GetWindowWidth(); // �؂蔲�����W�E
		scissorRects[i].top = 0; // �؂蔲�����W��
		scissorRects[i].bottom = scissorRects[i].top + winApp_->GetWindowHeight(); // �؂蔲�����W��
	}
	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	cmdList->RSSetViewports(2, viewports);
	
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	cmdList->RSSetScissorRects(2, scissorRects);


	//�r���[�|�[�g�̐ݒ�
	/*cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(
		0.0f, 0.0f, (float)winApp_->GetWindowWidth(), (float)winApp_->GetWindowHeight()));*/
	

	for (int32_t i = 0; i < 2; i++) {
		//�S��ʃN���A
		cmdList->ClearRenderTargetView(rtvHs[i], sCLEAR_COLOR, 0, nullptr);
	}
	

	//�[�x�o�b�t�@�N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	for (int32_t i = 0; i < 2; i++) {
		barrierDesc_.Transition.pResource = texBuff_[i].Get(); // �o�b�N�o�b�t�@���w��
		barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
		barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // �\����Ԃ�
		cmdList->ResourceBarrier(1, &barrierDesc_);
	}
	

	//���\�[�X�o���A��ύX
	//CD3DX12_RESOURCE_BARRIER resouceBarrier;



	//for (int32_t i = 0; i < 2; i++) {
	//	//resouceBarrier.Transition(texBuff_[i].Get(),
	//	//	D3D12_RESOURCE_STATE_RENDER_TARGET,
	//	//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//	//cmdList->ResourceBarrier(1, &resouceBarrier);

	//	resouceBarrier.Transition(texBuff_[i].Get(),
	//		D3D12_RESOURCE_STATE_RENDER_TARGET,
	//		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//	cmdList->ResourceBarrier(1, &resouceBarrier);
	//}
	

	


}
