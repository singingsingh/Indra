struct GeometricInputType
{
	float4 pos : POSITION;
	float2 size : SIZE;
};

struct GeometricOutputType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

[maxvertexcount(4)]
void WaveParticlesRTTGeometricShader(point GeometricInputType input[1], inout TriangleStream<GeometricOutputType> triStream)
{
	GeometricOutputType output;

	float halfWidth = input[0].size.x * 0.5;
	float halfHeight = input[0].size.y * 0.5;

	output.pos = float4(input[0].pos.x - halfWidth, input[0].pos.y - halfHeight, input[0].pos.z, input[0].pos.w);
	output.tex = float2(0.0, 1.0);
	triStream.Append(output);

	output.pos = float4(input[0].pos.x - halfWidth, input[0].pos.y + halfHeight, input[0].pos.z, input[0].pos.w);
	output.tex = float2(0.0, 0.0);
	triStream.Append(output);

	output.pos = float4(input[0].pos.x + halfWidth, input[0].pos.y - halfHeight, input[0].pos.z, input[0].pos.w);
	output.tex = float2(1.0, 1.0);
	triStream.Append(output);

	output.pos = float4(input[0].pos.x + halfWidth, input[0].pos.y + halfHeight, input[0].pos.z, input[0].pos.w);
	output.tex = float2(1.0, 0.0);
	triStream.Append(output);
}