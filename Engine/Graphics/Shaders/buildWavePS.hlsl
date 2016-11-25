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
	direction = (-input.norPos) * output.z;
	output.x = direction.x;
	output.y = direction.y;

	//float2 direction = float2(1.0, 1.0);

	//if (input.norPos.x < 0)
	//{
	//	direction.x = -1.0;
	//}
	//if (input.norPos.y < 0)
	//{
	//	direction.y = -1.0;
	//}

	//float var = sin(PI*distance);
	//output.x = -1.0 * direction.x * var * output.z;
	//output.y = -1.0 * direction.y * var * output.z;

    return output;
}
