struct GeometricInputType
{
	float2 pos : POSITION;
	float amplitude : AMPLITUDE;
	float radius : RADIUS;
};

struct GeometricOutputType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float amplitude : AMPLITUDE;
};

[maxvertexcount(4)]
void WaveParticlesRTTGeometricShader(point GeometricInputType input[1], inout TriangleStream<GeometricOutputType> triStream)
{
	GeometricOutputType output;
	output.amplitude = input[0].amplitude;

	output.pos = float4(input[0].pos.x - input[0].radius, input[0].pos.y - input[0].radius, 0.0, 1.0);
	output.tex = float2(0.0, 1.0);
	triStream.Append(output);

	output.pos = float4(input[0].pos.x - input[0].radius, input[0].pos.y + input[0].radius, 0.0, 1.0);
	output.tex = float2(0.0, 0.0);
	triStream.Append(output);

	output.pos = float4(input[0].pos.x + input[0].radius, input[0].pos.y - input[0].radius, 0.0, 1.0);
	output.tex = float2(1.0, 1.0);
	triStream.Append(output);

	output.pos = float4(input[0].pos.x + input[0].radius, input[0].pos.y + input[0].radius, 0.0, 1.0);
	output.tex = float2(1.0, 0.0);
	triStream.Append(output);
}