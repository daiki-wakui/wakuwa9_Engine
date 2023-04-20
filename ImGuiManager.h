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
	//�ˑ��N���X
	WindowsApp* winApp_ = nullptr;
	DirectXBasis* directXBase_ = nullptr;

	HRESULT result;
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

public:

	void Initialize(WindowsApp* winApp, DirectXBasis* directXBase);

	void Finalize();

	void Begin();

	void End();

	void Draw();
};

