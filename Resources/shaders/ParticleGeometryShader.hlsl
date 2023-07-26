#include "ParticleHeader.hlsli"

static const uint vnum = 4;
static const float4 offset_arrey[vnum] = {
	float4(-0.5f,-0.5f,0,0),	//左下
	float4(-0.5f,+0.5f,0,0),	//左上
	float4(+0.5f,-0.5f,0,0),	//右下
	float4(+0.5f,+0.5f,0,0)		//右上
};

static const float2 uv_arrey[vnum] = {
	float2(0,1),	//左下
	float2(0,0),	//左上
	float2(1,1),	//右下
	float2(1,0)		//右上
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;

	//4頂点まわす
	for (uint i = 0; i < vnum; i++) {
		float4 offset;

		offset = offset_arrey[i] * input[0].scale;
		offset = mul(matBillboard, offset);
		//float4 offsetl = mul(matBillboard,offset_arrey[i]);
		//ワールド座標ベースで、ずらす
		element.svpos = input[0].pos + offset;
		//ビュー、射影変換
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_arrey[i];
		output.Append(element);
	}

	/*element.normal = input[0].normal;
	element.uv = input[0].uv;
	element.svpos = input[0].svpos;
	output.Append(element);

	element.svpos = input[0].svpos + float4(10.0f, 10.0f, 0.0f, 0.0f);
	output.Append(element);

	element.svpos = input[0].svpos + float4(10.0f, 0.0f, 0.0f, 0.0f);
	output.Append(element);*/
	
	/*for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.svpos  = input[i].svpos;
		element.normal = input[i].normal;
		element.uv     = input[i].uv;
		output.Append(element);
	}
	output.RestartStrip();

	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.svpos = input[i].svpos + float4(20.0f, 0.0f, 0.0f, 0.0f);
		element.normal = input[i].normal;
		element.uv = input[i].uv * 5.0f;
		output.Append(element);
	}*/
}