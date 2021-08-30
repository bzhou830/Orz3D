//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
/*
Texture2D tex;

SamplerState splr;

float4 main(float2 tc : TEXCOORD) : SV_Target
{
	return tex.Sample(splr,tc);
}
*/
cbuffer CBuf
{
	float4 face_colors[6];
};

float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return face_colors[tid / 2];
}
