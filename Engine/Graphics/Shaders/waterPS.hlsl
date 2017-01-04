Texture2D shaderTexture;
TextureCube cubeMap;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseMatColor;
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
	float3 fromCamera : TEXCOORD1;
};

float4 WaterPixelShader(PixelInputType input) : SV_TARGET
{
    float3 lightDir;
    float lightIntensity;
    float4 retVal;
    float3 reflection;
    float4 specularColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 reflectionColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuseColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    lightDir = -lightDirection;
	float next = 1.0 / 1024.0;


	float damping = 0.01;

	float currentHeight = shaderTexture.Sample(SampleType, input.tex).z * damping;

	float2 right = float2(next, 0.0);
	float rtHeight = shaderTexture.Sample(SampleType, input.tex + right).z * damping;
	float2 top = float2(0.0, next);
	float topHeight = shaderTexture.Sample(SampleType, input.tex - top).z * damping;

	float3 xAxis = normalize(float3(next, rtHeight - currentHeight, 0.0));
	float3 zAxis = normalize(float3(0.0, topHeight - currentHeight, next));
	float3 normal = cross(zAxis, xAxis);
	//normal = mul(normal, (float3x3)worldMatrix);
	normal = normalize(normal);

	reflection = reflect(input.fromCamera, normal);
	
	lightIntensity = saturate(dot(normal, lightDir));
	lightIntensity = 1.0;
	if(lightIntensity > 0.0f)
    {
		diffuseColor = (diffuseMatColor * lightIntensity);
		specularColor = pow(saturate(dot(reflection, normal)), specularPower);
    }

	reflectionColor = cubeMap.Sample(SampleType, reflection) * 0.5 ;

	//reflectionColor = float4(normal.z, normal.z, normal.z, 1.0);

	retVal = ((ambientColor + diffuseColor) * reflectionColor) + specularColor;
	retVal.a = 0.5;
    return retVal;
}