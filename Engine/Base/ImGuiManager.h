#pragma once

#include "WindowsApp.h"
#include "DirectXBasis.h"

#include <wrl.h>
#include <imgui.h>

class ImGuiManager
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	//依存クラス
	WindowsApp* winApp_ = WindowsApp::GetInstance();
	DirectXBasis* directXBase_ = DirectXBasis::GetInstance();

	
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	ImGuiManager() = default;
	~ImGuiManager() = default;
	//コピーコンストラクタ無効
	ImGuiManager(const ImGuiManager& obj) = delete;
	//代入演算子を無効
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;


public:

	static ImGuiManager* GetInstance();


	void Initialize();

	void Finalize();

	void Begin();

	void End();

	void Draw();
};

