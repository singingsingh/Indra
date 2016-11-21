Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 WaveParticlesRTTPixelShader(PixelInputType input) : SV_TARGET
{
	float4 output;
	output.rgba = shaderTexture.Sample(SampleType, input.tex).rrra;
	return output;
}