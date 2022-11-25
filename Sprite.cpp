#include "Sprite.h"

void Sprite::Initialize(SpriteBasis* spBasis)
{
	this->spBasis = spBasis;

	VertexData();
}

void Sprite::Update()
{

}

void Sprite::Draw()
{
	// パイプラインステート設定
	spBasis->GetDxBasis()->GetCommandList()->SetPipelineState(spBasis->GetPipelineState());

	// ルートシグネチャの設定コマンド
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(spBasis->GetRootSignature());

	// プリミティブ形状の設定コマンド
	spBasis->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	spBasis->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	// 描画コマンド
	//dxBasis->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
	spBasis->GetDxBasis()->GetCommandList()->DrawInstanced(vertexSize, 1, 0, 0); // 全ての頂点を使って描画
}

void Sprite::VertexData()
{
#pragma region  頂点データ

	// 頂点データ
	XMFLOAT3 vertices[] = {
	{ -0.5f, -0.5f, 0.0f }, // 左下
	{ -0.5f, +0.5f, 0.0f }, // 左上
	{ +0.5f, -0.5f, 0.0f }, // 右下
	};
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	vertexSize = _countof(vertices);

#pragma endregion

#pragma region  頂点バッファの設定
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region  頂点バッファの生成

	// 頂点バッファの生成
	
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region  頂点バッファへのデータ転送

	//GPU上のバッファに対応した仮想メモリ
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
	}
	//繋がりを解除
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region  頂点バッファビューの生成

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(XMFLOAT3);

#pragma endregion
}
