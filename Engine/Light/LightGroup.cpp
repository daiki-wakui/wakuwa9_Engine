#include "LightGroup.h"
#include <assert.h>

using namespace DirectX;

//静的メンバ変数の実体
ID3D12Device* LightGroup::sDevice = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
{
	//再初期化チェック
	assert(!LightGroup::sDevice);

	//nullptrチェック
	assert(device);

	LightGroup::sDevice = device;
}

void LightGroup::Create()
{
	//3Dオブジェクトのインスタンスを生成
	//LightGroup* instance = std::make_unique<LightGroup>();

	//初期化
	//instance->Initialize();

	//return instance;
}

void LightGroup::Initialize()
{
	//nullptrチェック
	assert(sDevice);

	DefaultLightSetting();

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	HRESULT result;

	result = sDevice->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	assert(SUCCEEDED(result));

	//定数バッファへデータ転送
	TransferConstBuffer();
}

void LightGroup::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {

		//環境光
		constMap->ambientColor = ambientColor;

		//平行光源
		for (int32_t i = 0; i < sDIRLIGHT_NUM; i++) {
			//ライトが有効なら設定を転送
			if (dirLights[i].IsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
			}
			//ライトが無効ならライト色を0に
			else {
				constMap->dirLights[i].active = 0;
			}
		}

		//点光源
		for (int32_t i = 0; i < sPOINTLIGHT_NUM; i++) {
			//ライトが有効なら設定を転送
			if (pointLights[i].IsActive()) {
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightpos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightcolor = pointLights[i].GetLightcolor();
				constMap->pointLights[i].lightatten = pointLights[i].GetLightAtten();
			}
			//ライトが無効ならライト色を0に
			else {
				constMap->pointLights[i].active = 0;
			}
		}

		//スポットライト
		for (int32_t i = 0; i < sSPOTLIGHT_NUM; i++) {
			//ライトが有効なら設定を転送
			if (spotLights[i].IsActive()) {
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightv = -spotLights[i].GetLightDir();
				constMap->spotLights[i].lightpos = spotLights[i].GetLightPos();
				constMap->spotLights[i].lightcolor = spotLights[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightactoranglecos = spotLights[i].GetLightFactorAngleCos();
			}
			//ライトが無効ならライト色を0に
			else {
				constMap->spotLights[i].active = 0;
			}
		}

		//丸影
		for (int32_t i = 0; i < sCIRCLESHADOW_NUM; i++) {
			//ライトが有効なら設定を転送
			if (circleShadows[i].IsActive()) {
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = -circleShadows[i].GetDir();
				constMap->circleShadows[i].casterPos = circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distanseCasterLight = circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos = circleShadows[i].GetFactorAngleCos();
			}
			//ライトが無効ならライト色を0に
			else {
				constMap->circleShadows[i].active = 0;
			}
		}

		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[0].SetLightDir({ 0.0f, -1.0f, 0.0f, 0 });

	dirLights[1].SetActive(true);
	dirLights[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[1].SetLightDir({ +0.5f, +0.1f, +0.2f, 0 });

	dirLights[2].SetActive(true);
	dirLights[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[2].SetLightDir({ -0.5f, +0.1f, -0.2f, 0 });
}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{
	ambientColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);

	dirLights[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int32_t index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);

	dirLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(int32_t index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);

	dirLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);

	pointLights[index].SetActive(active);
}

void LightGroup::SetPointLightPos(int32_t index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);

	pointLights[index].SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetPointLightColor(int32_t index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);

	pointLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(int32_t index, const XMFLOAT3& lightatten)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);

	pointLights[index].SetLightAtten(lightatten);
	dirty = true;
}

void LightGroup::SetSpotLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);

	spotLights[index].SetActive(active);
}

void LightGroup::SetSpotLightDir(int32_t index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);

	spotLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetSpotLightPos(int32_t index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);

	spotLights[index].SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetSpotLightColor(int32_t index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);

	spotLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetSpotLightAtten(int32_t index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);

	spotLights[index].SetLightAtten(lightAtten);
	dirty = true;
}

void LightGroup::SetSpotLightFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);

	spotLights[index].SetLightFactorAngle(lightFactorAngle);
	dirty = true;
}

void LightGroup::SetCircleShadowActive(int32_t index, bool active)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);

	circleShadows[index].SetActive(active);
}

void LightGroup::SetCircleShadowCasterPos(int32_t index, const XMFLOAT3& casterPos)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);

	circleShadows[index].SetCasterPos(casterPos);
	dirty = true;
}

void LightGroup::SetCircleShadowDir(int32_t index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);

	circleShadows[index].SetDir(lightdir);
	dirty = true;
}

void LightGroup::SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);

	circleShadows[index].SetDistanceCasterLight(distanceCasterLight);
	dirty = true;
}

void LightGroup::SetCircleShadowAtten(int32_t index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);

	circleShadows[index].SetAtten(lightAtten);
	dirty = true;
}

void LightGroup::SetCircleShadowFactorAngle(int32_t index, const XMFLOAT2& lightFactorAngle)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);

	circleShadows[index].SetFactorAngle(lightFactorAngle);
	dirty = true;
}
