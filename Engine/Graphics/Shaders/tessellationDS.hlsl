cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct HullOutputType
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("quad")]

PixelInputType ColorDomainShader(ConstantOutputType input, float2 uvCoord : SV_DomainLocation, const OutputPatch<HullOutputType, 4> patch)
{
	float3 vertexPosition;
	PixelInputType output;

	// Determine the position of the new vertex.
	float3 v1 = lerp(patch[0].position, patch[1].position, uvCoord.x);
	float3 v2 = lerp(patch[2].position, patch[3].position, uvCoord.x);
	vertexPosition = lerp( v1, v2, uvCoord.y );

	// Calculate the position of the new vertex against the world, view, and projection matrices.
	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Send the input color into the pixel shader.
	float4 c1 = lerp(patch[0].color, patch[1].color, uvCoord.x);
	float4 c2 = lerp(patch[2].color, patch[3].color, uvCoord.x);
	output.color = lerp( c1, c2, uvCoord.y );

	return output;
}
