#include "Sprite.h"
using namespace DirectX;

void Sprite::Create(float x, float y)
{
	//ヒープ設定(color)
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//ヒープ設定(mat)
	D3D12_HEAP_PROPERTIES cbMatHeapProp{};
	cbMatHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbMatResourceDesc{};
	cbMatResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMatResourceDesc.Width = (sizeof(CounstBufferDataTransform) + 0xff) & ~0xff;
	cbMatResourceDesc.Height = 1;
	cbMatResourceDesc.DepthOrArraySize = 1;
	cbMatResourceDesc.MipLevels = 1;
	cbMatResourceDesc.SampleDesc.Count = 1;
	cbMatResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	//color
	//定数バッファの生成
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result_));

	//mat
	//定数バッファの生成
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbMatHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbMatResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));

	//定数バッファのマッピング(color)
	result_ = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result_));

	//定数バッファのマッピング(mat)
	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result_));

	//値を書きこんで自動転送
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);	//色変更

	//左上を原点に設定
	matProjection_ =
		XMMatrixOrthographicOffCenterLH(
			0.0f, (float)winApp_->GetWindowWidth(),
			(float)winApp_->GetWindowHeight(), 0.0f,
			0.0f, 1.0f);

	matWorld_ = XMMatrixIdentity();

	matScale_ = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld_ *= matScale_;

	matRot_ = XMMatrixIdentity();
	matRot_ *= XMMatrixRotationZ(XMConvertToRadians(0.0f));	//Z 0度回転
	matRot_ *= XMMatrixRotationX(XMConvertToRadians(0.0f));	//X 0度回転
	matRot_ *= XMMatrixRotationY(XMConvertToRadians(0.0f));	//Y 0度回転
	matWorld_ *= matRot_;

	matTrans_ = XMMatrixTranslation(x, y, 0.0f);
	matWorld_ *= matTrans_;

	constMapTransform_->mat = matWorld_ * matProjection_;

	SetPosition({ x,y });
}

void Sprite::Initialize(int32_t texIndex)
{
	spBasis_ = SpriteBasis::GetInstance();
	winApp_ = WindowsApp::GetInstance();

	if (texIndex != UINT32_MAX) {
		index_ = texIndex;
	}

	VertexData();
	IndexData();
}

void Sprite::Update()
{
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	//左右反転
	if (isFilpX_) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFilpY_) {
		top = -top;
		bottom = -bottom;
	}

	vertices_[LB].pos = { left ,bottom,0.0f };
	vertices_[LT].pos = { left ,top   ,0.0f };
	vertices_[RB].pos = { right,bottom,0.0f };
	vertices_[RT].pos = { right,top   ,0.0f };

	VertexDataTransfer();

	MatUpdate();
}

void Sprite::Draw(int32_t texNum)
{
	// パイプラインステート設定
	spBasis_->GetDxBasis()->GetCommandList()->SetPipelineState(spBasis_->GetPipelineState());

	// ルートシグネチャの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(spBasis_->GetRootSignature());

	spBasis_->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 定数バッファビュー(CBV)の設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { spBasis_->srvHeap_.Get() };
	spBasis_->GetDxBasis()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRVヒープの先頭アドレスを取得
	srvGpuHandle_ = spBasis_->srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle_.ptr += spBasis_->GetincrementSize() * texNum;

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle_);

	//定数バッファビュー(CBV)の設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);

	// インデックスバッファビューの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->IASetIndexBuffer(&ibView_);

	// 描画コマンド
	spBasis_->GetDxBasis()->GetCommandList()->DrawIndexedInstanced(indexSize_, 1, 0, 0, 0); // 全ての頂点を使って描画
}

void Sprite::VertexData()
{
#pragma region  頂点データ

	// 頂点データ
	vertices_[LB] = { {   0.0f, 100.0f, 0.0f },{ 0.0f , 1.0f } };
	vertices_[LT] = { {   0.0f, 0.0f, 0.0f }, { 0.0f , 0.0f } };
	vertices_[RB] = { { 100.0f, 100.0f, 0.0f },{ 1.0f , 1.0f } };
	vertices_[RT] = { { 100.0f,   0.0f, 0.0f },{ 1.0f , 0.0f } };

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));

	vertexSize_ = _countof(vertices_);

#pragma endregion

#pragma region  頂点バッファの設定
	// 頂点バッファの設定
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region  頂点バッファの生成

	// 頂点バッファの生成

	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp_, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

#pragma endregion

#pragma region  頂点バッファへのデータ転送

	VertexDataTransfer();

#pragma endregion

#pragma region  頂点バッファビューの生成

	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion
}

void Sprite::IndexData()
{
	//インデックスデータ
	uint16_t indices[] = {
		0 , 1 , 2,	//三角形1つ目
		1 , 2 , 3,	//三角形2つ目
	};

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	indexSize_ = _countof(indices);

	//リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeIB;	//インデックス情報が入る分のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	//インデックスバッファの生成
	result_ = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp_,
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int32_t i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	//マッピング解除
	indexBuff_->Unmap(0, nullptr);

	//インデックスバッファビューの生成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

void Sprite::MatUpdate()
{
	matWorld_ = XMMatrixIdentity();

	matRot_ = XMMatrixIdentity();
	matRot_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));	//Z 0度回転
	matRot_ *= XMMatrixRotationX(XMConvertToRadians(0));	//X 0度回転
	matRot_ *= XMMatrixRotationY(XMConvertToRadians(0));	//Y 0度回転
	matWorld_ *= matRot_;

	matTrans_ = XMMatrixTranslation(position_.x, position_.y, 0.0f);
	matWorld_ *= matTrans_;

	constMapTransform_->mat = matWorld_ * matProjection_;

	constMapMaterial->color = color_;	//色変更
}

void Sprite::RectTexture(const DirectX::XMFLOAT2& rectSize)
{
	ID3D12Resource* texBuff = spBasis_->GetTextureBuffer(index_);

	if (texBuff) {
		D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

		float tex_left = textureLeftTop_.x / (float)resDesc.Width;
		float tex_right = (textureLeftTop_.x + rectSize.x) / (float)resDesc.Width;
		float tex_top = textureLeftTop_.y / (float)resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + rectSize.y) / (float)resDesc.Height;

		vertices_[LB].uv = { tex_left,tex_bottom };
		vertices_[LT].uv = { tex_left,tex_top };
		vertices_[RB].uv = { tex_right,tex_bottom };
		vertices_[RT].uv = { tex_right,tex_top };

		size_ = rectSize;
	}

	VertexDataTransfer();

	MatUpdate();
}

void Sprite::VertexDataTransfer()
{
	//GPU上のバッファに対応した仮想メモリ
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	//全頂点に対して
	for (int32_t i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i];
	}
	//繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}

