cbuffer LightBuffer
{
	float currentTimeMS;
};

struct VertexInputType
{
	float4 data : DATA;	// x - SPAWNTIME, y - AMPLITUDE, z - RADIUS, w - VELOCITY
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

	//output.pos = input.origin;

	float elapsedTimeMS = currentTimeMS - input.data.x;
	output.pos = input.origin + (input.direction * input.data.w * elapsedTimeMS);

	output.amplitude = input.data.y;
	output.radius = input.data.z;

	return output;
}