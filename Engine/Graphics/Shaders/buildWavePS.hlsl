struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 norPos : NORMPOS;
};

float4 BuildWavePixelShader(PixelInputType input) : SV_TARGET
{
	const float PI = 3.14159265;
	const float twoPI = 6.2831853;

	float4 output = 1.0;

	float distance = sqrt((input.norPos.x*input.norPos.x) + (input.norPos.y*input.norPos.y));

	distance = min(distance, 1.0);
	output.z = 0.5 * (cos(PI * distance) + 1);

	float2 direction;
	direction = (-input.norPos) * output.z * sin(PI*distance);
	output.x = direction.x;
	output.y = direction.y;

    return output;
}
