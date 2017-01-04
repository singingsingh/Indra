#ifndef __BUILD_WAVE_SHADER_H__
#define	__BUILD_WAVE_SHADER_H__

#include <d3d11.h>
#include <stdint.h>

namespace Engine
{
	namespace Graphics
	{
		class BuildWaveShader
		{
			public:
				BuildWaveShader();
				~BuildWaveShader();
				void render(uint8_t indexCount);

			private :
				void initializeShader(const char* vsFilename, const char* psFilename);
				void outputShaderErrorMessage(ID3D10Blob*, const char * shaderFileName);

				ID3D11VertexShader* _vertexShader;
				ID3D11PixelShader* _pixelShader;
				ID3D11InputLayout* _layout;
		};
	}	// namespace Graphics
}	// namespace Enine

#endif	//__BUILD_WAVE_SHADER_H__
