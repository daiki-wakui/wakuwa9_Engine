#pragma once
#include "WindowsApp.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <cassert>
#include <vector>
#include <chrono>
#include <thread>

class DirectXBasis
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	WindowsApp* winApp_ = nullptr;

	HRESULT result_;
	ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGISwapChain4> swapChain_;
	ComPtr<ID3D12CommandAllocator> cmdAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	// �f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	// �X���b�v�`�F�[��
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	// �o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;

	D3D12_RESOURCE_BARRIER barrierDesc_{};

	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;
	//ID3D12DescriptorHeap* dsvHeap = nullptr;

	/*D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;*/

	ComPtr<ID3D12Resource> depthBuff_ = nullptr;

	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence_;
	//�t�F���X�l
	UINT64 fenceVal_ = 0;

	//���ԋL�^(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;

public:

	//DirectX�̊�Տ������֐�
	void Initialize(WindowsApp* winApp);
	void InitailizeDevice();
	void InitailizeCommand();
	void InitailizeSwapchain();
	void InitailizeRenderTargetView();
	void InitailizeDepthBuffer();
	void InitailizeFance();

	//FPS�Œ菉����
	void InitailizeFixFPS();

	//FPS�Œ�X�V
	void UpdateFixFPS();

	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	//�f�o�C�X�擾
	ID3D12Device* GetDevice() const { return device_.Get(); };
	//�R�}���h���X�g�擾
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); };

	//�o�b�N�o�b�t�@�̐����擾
	size_t GetBackBufferCount() const { return backBuffers_.size(); }
};

