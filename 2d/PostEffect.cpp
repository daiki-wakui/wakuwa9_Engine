#include "PostEffect.h"
#include <d3dx12.h>
#include <cassert>

const float PostEffect::sCLEAR_COLOR[4] = { 0.25f,0.5f,0.1f,0.0f };//緑っぽい色

void PostEffect::Initialize()
{
	HRESULT result;

	Sprite::Initialize(spBasis_, winApp_);


	//テクスチャバッファ設定
	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//テクスチャリソース設定
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		winApp_->GetWindowWidth(),
		(UINT)winApp_->GetWindowHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_CLEAR_VALUE clearValue;

	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	for (int32_t i = 0; i < 4; i++) {
		clearValue.Color[i] = sCLEAR_COLOR[i];
	}

	/*clearValue.Color[0] = sCLEAR_COLOR[0];
	clearValue.Color[1] = sCLEAR_COLOR[1];
	clearValue.Color[2] = sCLEAR_COLOR[2];
	clearValue.Color[3] = sCLEAR_COLOR[3];*/

	//テクスチャバッファの生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&texBuff_));

	assert(SUCCEEDED(result));


	//イメージデータ転送
	const UINT pixelCount = winApp_->GetWindowWidth() * winApp_->GetWindowHeight();

	const UINT rowPitch = sizeof(UINT) * winApp_->GetWindowHeight();

	const UINT deptPitch = rowPitch * winApp_->GetWindowHeight();

	UINT* img = new UINT[pixelCount];

	for (UINT i = 0; i < pixelCount; i++) {
		img[i] = 0xff0000ff;
	}

	result = texBuff_->WriteToSubresource(0, nullptr, img, rowPitch, deptPitch);
	assert(SUCCEEDED(result));
	delete[] img;

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;

	//SRV用デスクリプタヒープ生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&srvDescHeapDesc,
		IID_PPV_ARGS(&descHeapSRV_));

	assert(SUCCEEDED(result));


	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	spBasis_->GetDxBasis()->GetDevice()->CreateShaderResourceView(
		texBuff_.Get(),
		&srvDesc,
		descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
	);

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	//RTV用デスクリプタヒープ生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&rtvDescHeapDesc,
		IID_PPV_ARGS(&descHeapRTV));

	assert(SUCCEEDED(result));


	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//デスクリプタヒープにRTV生成
	spBasis_->GetDxBasis()->GetDevice()->CreateRenderTargetView(
		texBuff_.Get(),
		&renderTargetViewDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);


	//深度バッファリソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApp_->GetWindowWidth();
	depthResourceDesc.Height = winApp_->GetWindowHeight();
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//デプスステンシル

	// 深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//深度バッファの生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_));

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NumDescriptors = 1;

	//DRV用デスクリプタヒープ生成
	result = spBasis_->GetDxBasis()->GetDevice()->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV_));

	assert(SUCCEEDED(result));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	spBasis_->GetDxBasis()->GetDevice()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::SetDirectX(SpriteBasis* spBasis, WindowsApp* winApp)
{
	spBasis_ = spBasis;
	winApp_ = winApp;
}

PostEffect::PostEffect()
{

}

void PostEffect::Draw(int32_t texNum)
{
	// パイプラインステート設定
	spBasis_->GetDxBasis()->GetCommandList()->SetPipelineState(spBasis_->GetPipelineState());

	// ルートシグネチャの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootSignature(spBasis_->GetRootSignature());

	//プリニティブ形状を設定
	spBasis_->GetDxBasis()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

	// 定数バッファビュー(CBV)の設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV_.Get() };
	spBasis_->GetDxBasis()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRVヒープの先頭アドレスを取得
	/*srvGpuHandle_ = spBasis_->srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle_.ptr += spBasis_->GetincrementSize() * texNum;*/

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());

	//定数バッファビュー(CBV)の設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);

	// インデックスバッファビューの設定コマンド
	spBasis_->GetDxBasis()->GetCommandList()->IASetIndexBuffer(&ibView_);

	// 描画コマンド
	//dxBasis->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
	spBasis_->GetDxBasis()->GetCommandList()->DrawIndexedInstanced(indexSize_, 1, 0, 0, 0); // 全ての頂点を使って描画
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	// 1.リソースバリアで書き込み可能に変更
	barrierDesc_.Transition.pResource = texBuff_.Get(); // バックバッファを指定
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 表示状態から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	cmdList->ResourceBarrier(1, &barrierDesc_);

	/*CD3DX12_RESOURCE_BARRIER resouceBarrier;

	resouceBarrier.Transition(texBuff_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);*/

	//リソースバリアを変更
	//cmdList->ResourceBarrier(1,&resouceBarrier);

	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();


	//深度ステンシルビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV_->GetCPUDescriptorHandleForHeapStart();

	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// ビューポート設定コマンド
	//D3D12_VIEWPORT viewport{};
	//viewport.Width = (FLOAT)winApp_->GetWindowWidth();
	//viewport.Height = (FLOAT)winApp_->GetWindowHeight();
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.MinDepth = 0.0f;
	//viewport.MaxDepth = 1.0f;

	CD3DX12_VIEWPORT view;

	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width = (float)winApp_->GetWindowWidth();
	view.Height = (float)winApp_->GetWindowHeight();
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;

	// ビューポート設定コマンドを、コマンドリストに積む
	cmdList->RSSetViewports(1, &view);

	//ビューポートの設定
	/*cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(
		0.0f, 0.0f, (float)winApp_->GetWindowWidth(), (float)winApp_->GetWindowHeight()));*/

	

	// シザー矩形
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // 切り抜き座標左
	scissorRect.right = scissorRect.left + winApp_->GetWindowWidth(); // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + winApp_->GetWindowHeight(); // 切り抜き座標下
	// シザー矩形設定コマンドを、コマンドリストに積む
	cmdList->RSSetScissorRects(1, &scissorRect);

	

	//全画面クリア
	cmdList->ClearRenderTargetView(rtvH, sCLEAR_COLOR, 0, nullptr);

	//深度バッファクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 表示状態へ
	cmdList->ResourceBarrier(1, &barrierDesc_);

	//リソースバリアを変更
	/*CD3DX12_RESOURCE_BARRIER resouceBarrier;
	resouceBarrier.Transition(texBuff_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	cmdList->ResourceBarrier(1, &resouceBarrier);*/


}
