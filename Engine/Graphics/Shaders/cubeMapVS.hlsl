cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
    float3 position : POSITION;
};

struct VertexOutputType
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD;
};

VertexOutputType CubeMapVertexShader(VertexInputType input)
{
	VertexOutputType output;
    
	output.position = float4(input.position, 1.0);
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.position.z = output.position.w - 0.00001;
    
	output.tex = input.position;
    
    return output;
}