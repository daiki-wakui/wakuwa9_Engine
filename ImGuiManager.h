#pragma once
#include <wrl.h>
#include "WindowsApp.h"
#include "DirectXBasis.h"
#include <imgui.h>

class ImGuiManager
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	//依存クラス
	WindowsApp* winApp_ = nullptr;
	DirectXBasis* directXBase_ = nullptr;

	HRESULT result;
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

public:

	void Initialize(WindowsApp* winApp, DirectXBasis* directXBase);

	void Finalize();

	void Begin();

	void End();

	void Draw();
};

