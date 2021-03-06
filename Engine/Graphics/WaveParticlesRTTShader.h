#ifndef __WAVE_PARTICLE_RTT_SHADER_H__
#define	__WAVE_PARTICLE_RTT_SHADER_H__

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

namespace Engine
{
	namespace Graphics
	{
		class WaveParticlesRTTShader
		{
			public:

				WaveParticlesRTTShader();
				~WaveParticlesRTTShader();

				bool initialize();
				void shutdown();
				bool render(int vertexCount, ID3D11ShaderResourceView* texture, float currentTime);

			private:

				struct VSConstBufferType
				{
					float currentTime;
					float padding[3];
				};

				bool initializeShader(const char *vsFileName, const char *gsFileName, const char * psFileName);
				void outputShaderErrorMessage(ID3D10Blob*, const char * shaderFileName);

				bool setShaderParameters(ID3D11ShaderResourceView* texture, float currentTime);
				void renderShader(int vertexCount);

				ID3D11VertexShader* _vertexShader;
				ID3D11GeometryShader* _geometryShader;
				ID3D11PixelShader* _pixelShader;
				ID3D11InputLayout* _layout;
				ID3D11SamplerState* _sampleState;
				ID3D11Buffer* _vsConstBuffer;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__WAVE_PARTICLE_RTT_SHADER_H__