//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
/*
struct VSOut
{
	float2 tex : TEXCOORD;
	float4 pos : SV_Position;
};

cbuffer CBuf
{
	matrix transform;
};

VSOut main(float3 pos : Position, float2 tex : TEXCOORD)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f), transform);
	vso.tex = tex;
	return vso;
}


cbuffer CBuf
{
	matrix transform;
};

float4 main(float3 pos : Position) : SV_Position
{
	return mul(float4(pos, 1.0f), transform);
}
*/

cbuffer CBuf
{
	matrix model;
	matrix modelViewProj;
};

struct VSOut
{
	float3 worldPos : Position;
	float3 normal : Normal;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
	VSOut vso;
	vso.worldPos = (float3)mul(float4(pos, 1.0f), model);
	vso.normal = mul(n, (float3x3)model);
	vso.pos = mul(float4(pos, 1.0f), modelViewProj);
	return vso;
}