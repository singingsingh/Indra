Texture2D shaderTexture;
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

float4 tex3D(float3 tex)
{
	float4 retColor;
	float oneByEight = 0.125;

	float layer = saturate(tex.z);	// clamp
	// or to use wrap
	// value = fract(value);

	layer *= 63;

	float first = floor(layer);
	float second = first + 1.0;
	float weight = frac(layer);

	tex *= oneByEight;

	float4 color1 = shaderTexture.Sample(SampleType, tex + float2((first % 8)*oneByEight, (floor(first*oneByEight))*oneByEight));
	float4 color2 = shaderTexture.Sample(SampleType, tex + float2((second % 8)*oneByEight, (floor(second*oneByEight))*oneByEight));

	retColor = lerp(color1, color2, weight);
	return retColor;
}

float4 Pseudo3DTexPS(PixelInputType input) : SV_TARGET
{
    return tex3D(float3(input.tex, layer));
}