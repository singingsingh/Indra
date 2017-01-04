TextureCube shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 tex : TEXCOORD;
};

float4 CubeMapPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}
