//cbuffer LightBuffer
//{
//	double currentTick;
//};

struct VertexInputType
{
	float4 data : DATA;	// x - SPAWNTICK, y - AMPLITUDE, z - RADIUS, w - VELOCITY
	float2 origin : ORIGIN;
	float2 direction : DIRECTION;
};

struct VertexOutputType
{
	float2 pos : POSITION;
	float amplitude : AMPLITUDE;
	float radius : RADIUS;
};

VertexOutputType WaveParticlesRTTVertexShader(VertexInputType input)
{
	VertexOutputType output;

	output.pos = input.origin;
	output.amplitude = input.data.y;
	output.radius = input.data.z;

	return output;
}