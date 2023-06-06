#include "Sprite.h"

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
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//mat
	//定数バッファの生成
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&cbMatHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbMatResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング(color)
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result));

	//定数バッファのマッピング(mat)
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	//値を書きこんで自動転送
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);	//色変更

	//左上を原点に設定
	matProjection =
		XMMatrixOrthographicOffCenterLH(
			0.0f, winApp->GetWindowWidth(),
			(float)winApp->GetWindowHeight(), 0.0f,
			0.0f, 1.0f);

	matWorld = XMMatrixIdentity();

	matScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld *= matScale;

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));	//Z 0度回転
	matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));	//X 0度回転
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));	//Y 0度回転
	matWorld *= matRot;

	matTrans = XMMatrixTranslation(x, y, 0.0f);
	matWorld *= matTrans;

	constMapTransform->mat = matWorld * matProjection;

	XMFLOAT2 pos = { 0,0 };

}

void Sprite::Initialize(SpriteBasis* spBasis, WindowsApp* winApp)
{
	this->spBasis = spBasis;
	this->winApp = winApp;

	VertexData();
	IndexData();
}

void Sprite::Update()
{
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	vertices[LB].pos = { left ,bottom,0.0f };
	vertices[LT].pos = { left ,top   ,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top   ,0.0f };

	//GPU上のバッファに対応した仮想メモリ
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
	}
	//繋がりを解除
	vertBuff->Unmap(0, nullptr);


	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_));	//Z 0度回転
	matRot *= XMMatrixRotationX(XMConvertToRadians(0));	//X 0度回転
	matRot *= XMMatrixRotationY(XMConvertToRadians(0));	//Y 0度回転
	matWorld *= matRot;

	matTrans = XMMatrixTranslation(position_.x, position_.y, 0.0f);
	matWorld *= matTrans;

	constMapTransform->mat = matWorld * matProjection;

}

void Sprite::Draw(int texNum)
{
	// パイプラインステート設定
	spBasis->GetDxBasis()->GetCommandList()->SetPipelineState(spBasis->GetPipelineState());

	// ルートシグネチャの設定コマンド
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(spBasis->GetRootSignature());

	spBasis->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 定数バッファビュー(CBV)の設定コマンド
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { spBasis->srvHeap.Get() };
	spBasis->GetDxBasis()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRVヒープの先頭アドレスを取得
	srvGpuHandle = spBasis->srvHeap->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += spBasis->GetincrementSize() * texNum;

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//定数バッファビュー(CBV)の設定コマンド
	spBasis->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	spBasis->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	// インデックスバッファビューの設定コマンド
	spBasis->GetDxBasis()->GetCommandList()->IASetIndexBuffer(&ibView);

	// 描画コマンド
	//dxBasis->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
	spBasis->GetDxBasis()->GetCommandList()->DrawIndexedInstanced(indexSize, 1, 0, 0, 0); // 全ての頂点を使って描画
}

void Sprite::VertexData()
{
#pragma region  頂点データ

	// 頂点データ
	vertices[LB] = { {   0.0f, 100.0f, 0.0f },{ 0.0f , 1.0f } };
	vertices[LT] = { {   0.0f, 0.0f, 0.0f }, { 0.0f , 0.0f } };
	vertices[RB] = { { 100.0f, 100.0f, 0.0f },{ 1.0f , 1.0f } };
	vertices[RT] = { { 100.0f,   0.0f, 0.0f },{ 1.0f , 0.0f } };

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	vertexSize = _countof(vertices);

#pragma endregion

#pragma region  頂点バッファの設定
	// 頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
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
	Vertex* vertMap = nullptr;
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
	vbView.StrideInBytes = sizeof(vertices[0]);

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

	indexSize = _countof(indices);

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	//インデックスバッファの生成
	result = spBasis->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの生成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

