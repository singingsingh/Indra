Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

cbuffer WaterBuffer
{
	float4 waterColor;
	matrix worldMatrix;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 viewDirection : TEXCOORD1;
};

float4 WaterPixelShader(PixelInputType input) : SV_TARGET
{
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;

	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	// Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Invert the light direction for calculations.
    lightDir = -lightDirection;

	float next = 1.0 / 1024.0;

	float currentHeight = shaderTexture.Sample(SampleType, input.tex).z;
	float2 right = float2(next, 0.0);
	float rtHeight = shaderTexture.Sample(SampleType, input.tex + right).z;
	float2 top = float2(0.0, next);
	float topHeight = shaderTexture.Sample(SampleType, input.tex + top).z;

	float3 xAxis = normalize(float3(next, rtHeight - currentHeight, 0.0));
	float3 zAxis = normalize(float3(0.0, topHeight - currentHeight, next));

	float3 normal = normalize (cross(zAxis, xAxis));


    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(normal, lightDir));
	
	if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
				
        // Saturate the ambient and diffuse color.
        color = saturate(color);
		
        // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * normal - lightDir);

        // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);		
    }

    // Multiply the texture pixel and the input color to get the textured result.
    color = color * waterColor;

    // Add the specular component last to the output color.
    color = saturate(color + specular);

    return color;
}