struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 norPos : NORMPOS;
};

float4 BuildWavePixelShader(PixelInputType input) : SV_TARGET
{
	const float PI = 3.14159265;

	float4 output;
	output.w = 1.0;

	float distance = sqrt((input.norPos.x*input.norPos.x) + (input.norPos.y*input.norPos.y));
	distance = min(distance, 1.0);
	output.z = 0.5 * (cos(PI * distance) + 1);
	output.x = output.z;
	output.y = output.x;
	//output = float4(input.norPos.x, input.norPos.y, 0, 1);
    return output;
}
