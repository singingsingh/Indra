Texture3D shaderTexture;
SamplerState SampleType;

cbuffer TextureBuffer
{
	float layer;
	float3 padding;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 Pseudo3DTexPS(PixelInputType input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, float3(input.tex,layer));
}