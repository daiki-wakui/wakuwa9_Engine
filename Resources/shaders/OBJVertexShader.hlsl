#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	/*float3 lightdir = float3(1, -1, 1);
	lightdir = normalize(lightdir);

	float3 lightcolor = float3(1, 1, 1);*/

	//法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	//環境反射光
	float3 ambient = m_ambient;

	//拡散反射光
	float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;

	//鏡面反射光
	//視点座標
	//const float3 eye = float3(0, 0, -20);
	//光沢度
	const float3 shininess = 4.0f;
	//頂点から視点への方向ベクトル
	//float3 eyedir = normalize(eye - pos.xyz);
	float3 eyedir = normalize(cameraPos - wpos.xyz);

	//反射光ベクトル
	float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);


	//鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;


	//ピクセルシェーダに渡す値
	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);

	//全て加算
	output.color.rgb = (ambient + diffuse + specular) * lightcolor;

	//output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
	output.color.a = m_alpha;
	output.uv = uv;
	return output;
}