#pragma once
#include "SpriteBasis.h"
#include <DirectXMath.h>
using namespace DirectX;

class Sprite
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result;

	SpriteBasis* spBasis;

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	UINT vertexSize;

public:
	

	void Initialize(SpriteBasis* spBasis);
	void Update();
	void Draw();

	void VertexData();
};

