struct VertexInputType
{
    float2 position : POSITION;
};

struct VertexOutputType
{
    float4 position : SV_POSITION;
	float2 norPos : NORMPOS;
};

VertexOutputType BuildWaveVeretxShader(VertexInputType input)
{
	VertexOutputType output;
	output.position = float4(input, 0.0, 1.0);
	output.norPos = input.position;
    return output;
}