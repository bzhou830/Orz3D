//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
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

