#pragma once
#include "DirectXBasis.h"
#include <string>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

class SpriteBasis
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result;

	DirectXBasis* dxBasis = nullptr;


public:
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;

	void Initialize(DirectXBasis* dxBasis);
	void Update();
	void Draw();
	void Setting();

	void LoadShader();

	DirectXBasis* GetDxBasis() const { return dxBasis; };
	ID3D12RootSignature* GetRootSignature() const { return rootSignature.Get(); };
	ID3D12PipelineState* GetPipelineState() const { return pipelineState.Get(); };
};

