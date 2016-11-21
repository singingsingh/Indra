cbuffer MatrixBuffer
{
	matrix orthoViewMatrix;
	matrix orthoProjMatrix;
};

struct VertexInputType
{
	float2 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct VertexOutputType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VertexOutputType WaveParticlesRTTVertexShader(VertexInputType input)
{
	VertexOutputType output;

	output.pos = float4(input.pos, 0.0, 1.0);
	output.pos = mul(output.pos, orthoViewMatrix);
	output.pos = mul(output.pos, orthoProjMatrix);

	output.tex = input.tex;

	return output;
}