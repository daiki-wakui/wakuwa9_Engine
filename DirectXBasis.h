#pragma once
#include "WindowsApp.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <cassert>
#include <vector>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectXBasis
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	WindowsApp* winApp = nullptr;

	HRESULT result;
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// �f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// �X���b�v�`�F�[��
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// �o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers;


	D3D12_RESOURCE_BARRIER barrierDesc{};


	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;
	//�t�F���X�l
	UINT64 fenceVal = 0;
public:

	void Initialize(WindowsApp* winApp);
	void InitailizeDevice();
	void InitailizeCommand();
	void InitailizeSwapchain();
	void InitailizeRenderTargetView();
	void InitailizeDepthBuffer();
	void InitailizeFance();

	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	//�f�o�C�X�擾
	ID3D12Device* GetDevice() const { return device.Get(); };
	//�R�}���h���X�g�擾
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); };
};

