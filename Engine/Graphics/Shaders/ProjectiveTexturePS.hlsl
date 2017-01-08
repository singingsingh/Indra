Texture2D shaderTexture : register(t0);
Texture2D projectionTexture : register(t1);

SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 viewPosition : TEXCOORD1;
};

float4 ProjectiveTexturePS(PixelInputType input) : SV_TARGET
{
	float4 color;
	float3 lightDir;
	float lightIntensity;
	float4 textureColor;
	float2 projectTexCoord;
	float4 projectionColor;

	color = ambientColor;
	lightDir = -lightDirection;
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
	}

	color = saturate(color);

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	color = color * textureColor;

	// Calculate the projected texture coordinates.
	projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		projectionColor = projectionTexture.Sample(SampleType, projectTexCoord);
		color = projectionColor;
	}

	return color;
}