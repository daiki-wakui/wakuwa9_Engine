#pragma once
#include "WindowsApp.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <cassert>
#include <vector>
#include <chrono>
#include <thread>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"winmm.lib")

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

	// デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	// スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	D3D12_RESOURCE_BARRIER barrierDesc{};

	// フェンスの生成
	ComPtr<ID3D12Fence> fence;
	//フェンス値
	UINT64 fenceVal = 0;

	//時間記録(FPS固定用)
	std::chrono::steady_clock::time_point reference;

public:

	//DirectXの基盤初期化関数
	void Initialize(WindowsApp* winApp);
	void InitailizeDevice();
	void InitailizeCommand();
	void InitailizeSwapchain();
	void InitailizeRenderTargetView();
	void InitailizeDepthBuffer();
	void InitailizeFance();

	//FPS固定初期化
	void InitailizeFixFPS();

	//FPS固定更新
	void UpdateFixFPS();

	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();

	//デバイス取得
	ID3D12Device* GetDevice() const { return device.Get(); };
	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); };
};

