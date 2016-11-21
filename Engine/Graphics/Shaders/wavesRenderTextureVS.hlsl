cbuffer MatrixBuffer
{
	matrix orthoViewMatrix;
	matrix orthoProjMatrix;
};

struct VertexInputType
{
	float2 pos : POSITION;
	float2 size : SIZE;
};

struct VertexOutputType
{
	float4 pos : POSITION;
	float2 size : SIZE;
};

VertexOutputType WaveParticlesRTTVertexShader(VertexInputType input)
{
	VertexOutputType output;

	output.pos = float4(input.pos, 0.0, 1.0);
	output.pos = mul(output.pos, orthoViewMatrix);
	output.pos = mul(output.pos, orthoProjMatrix);

	output.size = input.size;

	return output;
}