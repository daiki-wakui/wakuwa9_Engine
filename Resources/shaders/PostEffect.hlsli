//マテリアル
cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;	//色(RGBA)
    float power;
};

//3D変換行列
cbuffer CounstBufferDataTransform : register(b1)
{
	matrix mat;		//3D変換行列
}


//頂点データの出力構造体
struct VSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	//uv値
	float2 uv : TEXCOORD;
};