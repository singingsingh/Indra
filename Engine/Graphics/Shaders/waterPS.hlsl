Texture2D shaderTexture;
TextureCube cubeMap;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

cbuffer WaterBuffer
{
	float4 waterColor;
	matrix worldMatrix;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 viewDirection : TEXCOORD1;
};

float4 WaterPixelShader(PixelInputType input) : SV_TARGET
{
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;

    color = ambientColor;
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    lightDir = -lightDirection;
	float next = 1.0 / 1024.0;

	float currentHeight = shaderTexture.Sample(SampleType, input.tex).z;
	float2 right = float2(next, 0.0);
	float rtHeight = shaderTexture.Sample(SampleType, input.tex + right).z;
	float2 top = float2(0.0, next);
	float topHeight = shaderTexture.Sample(SampleType, input.tex + top).z;

	float3 xAxis = normalize(float3(next, rtHeight - currentHeight, 0.0));
	float3 zAxis = normalize(float3(0.0, topHeight - currentHeight, next));

	float3 normal = normalize (cross(zAxis, xAxis));
	float4 reflectionColor = float4(1.0, 1.0, 1.0, 1.0);

    lightIntensity = saturate(dot(normal, lightDir));
	
	if(lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
        color = saturate(color);
        reflection = normalize(2 * lightIntensity * normal - lightDir);
		//reflectionColor = shaderTexture.Sample(SampleType, reflection);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);		
    }

    color = color * waterColor * reflectionColor;
    color = saturate(color + specular);

    return color;
}