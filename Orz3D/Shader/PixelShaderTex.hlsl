//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
Texture2D tex;

SamplerState splr;

float4 main(float2 tc : TEXCOORD) : SV_Target
{
	return tex.Sample(splr,tc);
}
