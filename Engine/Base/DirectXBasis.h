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

	// デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	// スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;

	D3D12_RESOURCE_BARRIER barrierDesc_{};

	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;
	//ID3D12DescriptorHeap* dsvHeap = nullptr;

	/*D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;*/

	ComPtr<ID3D12Resource> depthBuff_ = nullptr;

	// フェンスの生成
	ComPtr<ID3D12Fence> fence_;
	//フェンス値
	UINT64 fenceVal_ = 0;

	//時間記録(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

private:	//シングルートン

	DirectXBasis() = default;
	~DirectXBasis() = default;
	//コピーコンストラクタ無効
	DirectXBasis(const DirectXBasis& obj) = delete;
	//代入演算子を無効
	DirectXBasis& operator=(const DirectXBasis& obj) = delete;

public:

	static DirectXBasis* GetInstance();

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
	ID3D12Device* GetDevice() const { return device_.Get(); };
	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); };

	//バックバッファの数を取得
	size_t GetBackBufferCount() const { return backBuffers_.size(); }
};

