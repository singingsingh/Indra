Texture2D shaderTexture;
SamplerState SampleType;

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

struct VertexInputType
{
    float2 position : POSITION;
    float2 tex : TEXCOORD;
};

struct VertexOutputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
    float3 fromCamera : TEXCOORD1;
};

VertexOutputType WaterVertexShader(VertexInputType input)
{
	VertexOutputType output;
    float4 worldPosition;

	float4 height = shaderTexture[input.tex*1024];
	float4 pos = float4(input.position.x + height.x, height.z, input.position.y + height.y, 1.0);

    output.position = mul(pos, worldMatrix);
	output.fromCamera = normalize(output.position.xyz - cameraPosition.xyz);

    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input.tex;
	
    return output;
}
